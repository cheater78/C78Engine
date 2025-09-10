#pragma once
#include "Line.h"

namespace C78E::Math {

	/**
	 * @brief Plane, a plane in a dim dimensional space, represented by a normal vector and a distance
	 * @tparam dim dimensionality of the Plane
	 */
	template<Dimension dim>
	struct Plane : private HomogeneousCoordinate<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using HC = HomogeneousCoordinate<dim>;
	public:
		Plane() = default;
		Plane(VectorD normal, scalar distance = 0.f) : HC(normal.getDirection(), distance) { normalize(); }
		Plane(PointD support) : HC(support.getOriginVector().normalize().getDirection(), support.getOriginVector().length()) { normalize(); }
		Plane(std::initializer_list<scalar> list) : HC(list, 0.f) { } //TODO: is that right?
		Plane(Plane&) = default;
		Plane(const Plane&) = default;
		~Plane() = default;

		inline void setNormal(const VectorD& normal) {
			HC::setCoordinate(normal.getDirection());
			normalize();
		}
		inline void setDistance(scalar distance) {
			HC::setScale(distance);
		}

		inline VectorD getNormal() const {
			return HC::getCoordinate();
		}
		inline scalar getDistance() const {
			return HC::getScale();
		}

		inline void setSupport(const PointD& support) {
			setNormal(glm::normalize(support.getPosition()));
			setDistance(glm::length(support.getPosition()));
		}
		inline PointD getSupport() const {
			return PointD(getNormal() * getDistance());
		}

		inline bool isDegenerate() const {
			return getNormal() == VectorD();
		}
		inline operator bool() const {
			return !isDegenerate();
		}
		inline bool operator==(const Plane& other) const {
			return HC::getHomogeneousCoordinate() == other.getHomogeneousCoordinate();
		}

		Plane flip() const {
			return Plane(-getNormal(), -getDistance());
		}
	protected:
		virtual void normalize() {
			if(isDegenerate()) return; // skip if degenerate
			if(getNormal().length() == 1.f) return; // skip if already normalized
			// keep ax + by + cz = d, normalizing (a b c)T by l means to also divide d by l!
			const scalar normalLength = getNormal().length();
			HC::setHomogeneousCoordinate(HC::getHomogeneousCoordinate() * (1.f / normalLength));
		}
	private:
		// HomogeneousCoordinate
	};

	/**
	 * @brief Transforms a Plane given a TransformMatrix
	 * @tparam native dimensionality of the Plane
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param plane the Plane to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Plane
	 */
	template<Dimension native, Dimension transforming>
	Plane<native> transform(const Plane<native>& plane, const mat<transforming>& t) {
		// Refit transformation matrix to match the native + 1 dimension
		const mat<native + 1> nativeTransform = Math::refitTransformMatrix<transforming, native + 1>(t);
		// Transform the normal using inverse transpose
		const mat<native + 1> invTNativeTransform = glm::inverseTranspose(nativeTransform);

		// Transform a support point on the plane
		const Point<native> supportPoint = plane.getSupport();
		const Point<native> transformedSupportPoint = Math::transform(supportPoint, nativeTransform);
		const Vector<native> planeNormal = plane.getNormal();
		const Vector<native> transformedNormal = Math::transform(planeNormal, invTNativeTransform);

		// Recalculate distance using the dot product: distance = dot(n, p)
		const scalar newDistance = transformedNormal.dot(transformedSupportPoint.getOriginVector());
		return Plane(transformedNormal.normalize(), newDistance);
	}

	template<Dimension dim>
	using PlanePointIntersection = Intersection<dim, Point<dim>>;

	template<Dimension dim>
	static PlanePointIntersection<dim> isInFrontOf(const Plane<dim>& plane, const Point<dim>& position) {
		const Vector positionPlaneLocal = position - plane.getSupport();
		const scalar ang = positionPlaneLocal.dot(plane.getNormal());
		if(ang < 0.f) return PlanePointIntersection<dim>::createNone(); // negative normal scaling -> behind the plane
		if(ang == 0.f) return PlanePointIntersection<dim>::createIdentical(); // 0 normal scaling -> on the plane
		return PlanePointIntersection<dim>::createIntersection(position); // positive normal scaling -> in front of the plane
	}

	template<Dimension dim>
	using PlaneLineIntersection = Intersection<dim, Point<dim>>;

	template<Dimension dim>
	static PlaneLineIntersection<dim> intersection(const Plane<dim>& plane, const Line<dim>& line) {
		const float denom = glm::dot(plane.getNormal(), line.getDirection());
		if(isNearZero(denom)) {
			if(denom == 0.f && glm::dot(plane.getNormal(), line.getSupport()) == plane.getDistance()) // Line is on the Plane
				return PlaneLineIntersection<dim>::createIdentical();
			return PlaneLineIntersection<dim>::createNone(); // Line is parallel to the plane
		}
		const float t = -(glm::dot(plane.getNormal(), line.getSupport()) + plane.getDistance()) / denom;
		return PlaneLineIntersection<dim>::createIntersection(line.getSupport() + t * line.getDirection());
	}

	template<Dimension dim>
	using PlanePlaneIntersection = Intersection<dim, Line<dim>>;

	template<Dimension dim>
	static PlanePlaneIntersection<dim> intersection(const Plane<dim>& plane1, const Plane<dim>& plane2) {
		if(plane1 == plane2) {
			return PlanePlaneIntersection<dim>::createIdentical();
		}

		const vec<dim>& n1 = plane1.getNormal();
		const vec<dim>& n2 = plane2.getNormal();
		const scalar d1 = plane1.getDistance();
		const scalar d2 = plane2.getDistance();

		matNxM<2, dim> A = glm::transpose(matNxM<dim, 2>{ n1, n2 }); // normal vectors as rows
		vec2 b{ d1, d2 }; // distances

		scalar det = glm::determinant(A);
		if(isNearZero(det)) return PlanePlaneIntersection<dim>::createNone(); // planes are parallel

		const vec<dim> support = glm::inverse(A) * b; // solve for the support point
		const Point<dim> supportPoint(support);

		Line<dim> intersection(Vector<dim>(), supportPoint);
		if constexpr(dim != 2) {
			intersection.setDirection(Vector<dim>(glm::cross(n1, n2)).normalize()); // normal of the intersection line
		}
		return PlanePlaneIntersection<dim>::createIntersection(intersection);
	}

	/**
	 * @brief PlanesCount, the number of Planes in an array
	 */
	using PlanesCount = size_t;

	/**
	 * @brief Planes, an array of Planes of the same Dimension
	 * @tparam dim the Dimension of the Planes
	 * @tparam size the number of Planes in the array
	 */
	template<Dimension dim, PlanesCount size>
		requires (dim >= 2)
	struct Planes : public std::array<Plane<dim>, size> {
	public:
		using Count = PlanesCount;
	public:
		Planes() = default;
		Planes(std::initializer_list<Plane<dim>> list) : std::array<Plane<dim>, size>(list) { }
		~Planes() = default;
	};

	/**
	 * @brief Transforms a Planes array given a TransformMatrix
	 * @tparam dim dimensionality of the Planes
	 * @tparam size the number of Planes in the array
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param planes the Planes to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Planes
	 */
	template<Dimension dim, PlanesCount size, Dimension transforming>
		requires (dim >= 2)
	Planes<dim, size> transform(const Planes<dim, size>& planes, const mat<transforming>& t) {
		Planes<dim, size> transformedPlanes;
		for(PlanesCount i = 0; i < size; i++) {
			transformedPlanes[i] = transform(planes[i], t);
		}
		return transformedPlanes;
	}

	/**
	 * @brief DynamicPlanes, dynamic array of Planes of the same Dimension
	 * @tparam dim the Dimension of the Planes
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicPlanes : public std::vector<Plane<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicPlanes() = default;
		DynamicPlanes(std::initializer_list<Plane<dim>> init) {
			this->resize(init.size());
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicPlanes() = default;
	};

	/**
	 * @brief Transforms a dynamic Planes array given a TransformMatrix
	 * @tparam dim dimensionality of the Planes
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param planes the Planes to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Planes
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicPlanes<dim> transform(const DynamicPlanes<dim>& planes, const mat<transforming>& t) {
		DynamicPlanes<dim> transformedPlanes;
		transformedPlanes.resize(planes.size());
		for(size_t i = 0; i < planes.size(); i++) {
			transformedPlanes[i] = transform(planes[i], t);
		}
		return transformedPlanes;
	}
	
}