#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	/**
	 * @brief HullCollider, for convex Meshes
	 * @tparam dim 
	 */
	template<Dimension dim>
	struct HullCollider : public AABBCollider<dim> {
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		template<PointsCount count>
		using Points = Points<dim, count>;
		template<VectorsCount count>
		using Vectors = Vectors<dim, count>;
		using DynamicPoints = DynamicPoints<dim>;
		using DynamicVectors = DynamicVectors<dim>;
		using Quad = Quad<dim>;
		using AABB = AABB<dim>;
		using Collider = Collider<dim>;
		using AABBCollider = AABBCollider<dim>;
		using Transform = Transform<dim>;
	public:
		template<PointsCount count>
		static HullCollider<dim> createStatic(const Points<count>& points) {
			HullCollider<dim> collider;
			collider.m_HullPoints.reserve(count);
			for(size_t i = 0; i < count; i++) {
				collider.addPoint(points[i]);
			}
			//TODO: check convexity!
			return collider;
		}
		static HullCollider<dim> createTriangle() {
			return createStatic(Triangle::getUnitTriangle());
		}
		static HullCollider<dim> createSquare() {
			return createStatic(Quad::getUnitQuadPoints());
		}
		static HullCollider<dim> createTetrahedon() {
			return createStatic(Tetrahedon::getUnitTetrahedon());
		}
		static HullCollider<dim> createCube() {
			return createStatic(Cube::getUnitCube());
		}

	
	public:
		HullCollider() = default;
		HullCollider(const DynamicPoints& points) : AABBCollider(), m_HullPoints(points) {
			calcBounds();
		}
		HullCollider(HullCollider&) = default;
		HullCollider(const HullCollider&) = default;
		~HullCollider() = default;

		virtual void addPoint(const Point& p) {
			m_HullPoints.push_back(p);
			AABB::growToInclude(p);
		}
		virtual void removePoint(const Point& p) {
			m_HullPoints.erase(std::find(m_HullPoints.begin(), m_HullPoints.end(), p));
			calcBounds();
		}

		virtual const DynamicPoints& getPoints() const {
			return m_HullPoints;
		}

		AABB& calcBounds() {
			AABB::reset();
			for(const Point& point : m_HullPoints) {
				AABB::growToInclude(point);
			}
			return *this;
		}

		/**
		 * @brief calculates the nearest surface point on the Mesh to the given point
		 * @param transformToPointSpace the Meshes Transform in the points space, probably world space -> the Meshes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			Points<dim> closestPoints; // closest Surface to point
			std::array<scalar, dim> closestDistances;
			closestDistances.fill(+scalar_limit::infinity());
			
			for(const Point& hullPointLocal : m_HullPoints) {
				const Point hullPoint = Math::transform(hullPointLocal, transformToPointSpace.toMat());
				const scalar pointDistance = (hullPoint - point).length();
				for(size_t i = 0; i < dim; i++) {
					if(pointDistance < closestDistances[i]) {
						for(size_t j = dim - 1; j > i; j--) {
							closestDistances[j] = closestDistances[j - 1];
							closestPoints[j] = closestPoints[j - 1];
						}
						closestDistances[i] = pointDistance;
						closestPoints[i] = hullPoint;
						break;
					}
				}
			}

			const Vector pointSurfaceLocal = point - closestPoints[0];
			Vectors<dim - 1> surfaceVectors;
			for(VectorsCount i = 0; i < dim - 1; i++) {
				surfaceVectors[i] = closestPoints[i + 1] - closestPoints[0];
			}
			const Vector surfaceNormal = Math::cross<dim>(surfaceVectors);
			const Vector pointSurfaceNormalComponentSurfaceLocal = pointSurfaceLocal.projectOn(surfaceNormal);

			Vector nearSurfacePointSurfaceLocal = pointSurfaceLocal - pointSurfaceNormalComponentSurfaceLocal;

			for(VectorsCount i = 0; i < dim - 1; i++) {
				const scalar projection = nearSurfacePointSurfaceLocal.dot(surfaceVectors[i]) / surfaceVectors[i].lengthSquared();
				if(projection < 0.f) {
					nearSurfacePointSurfaceLocal += surfaceVectors[i] * -projection;
				} else if(projection > 1.f) {
					nearSurfacePointSurfaceLocal += surfaceVectors[i] * (1.f - projection);
				}
			}
			const Point nearSurfacePoint = closestPoints[0] + nearSurfacePointSurfaceLocal;
			return nearSurfacePoint;
		}

		/**
		 * @brief 
		 * @param transformToVectorSpace 
		 * @param direction 
		 * @return 
		 */
		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			const Vector directionLocal = Math::transform(direction, transformToVectorSpace.toInvMat());

			Point maxPoint = Point(0.f);
			scalar maxDistance = -scalar_limit::infinity();
			for(const Point& point : m_HullPoints) {
				const scalar distance = directionLocal.dot(point.getOriginVector());
				if(distance > maxDistance) {
					maxDistance = distance;
					maxPoint = point;
				}
			}
			return Math::transform(maxPoint, transformToVectorSpace.toMat());
		}

		virtual AABB getBounds() const override {
			return AABBCollider::getBounds();
		}

		virtual Collider::Type getType() const { return Collider::Type::Hull; }
	public:
		DynamicPoints m_HullPoints;
	};

	template<Dimension dim>
	HullCollider<dim> transform(const HullCollider<dim>& collider, const Transform<dim>& transform) {
		return HullCollider<dim>(Math::transform(collider.getPoints(), transform.toMat()));
	}

	/**
	 * @brief DynamicHullColliders, 
	 * @tparam dim
	 */
	template<Dimension dim>
	struct DynamicHullColliders : public std::vector<HullCollider<dim>> {
	public:
		DynamicHullColliders() = default;
		DynamicHullColliders(std::initializer_list<HullCollider<dim>> hullColliders) : std::vector<HullCollider<dim>>() {
			this->reserve(hullColliders.size());
			for(const HullCollider<dim>& hullCollider : hullColliders) {
				this->emplace_back(hullCollider);
			}
		}
		DynamicHullColliders(DynamicHullColliders&) = default;
		DynamicHullColliders(const DynamicHullColliders&) = default;
		~DynamicHullColliders() = default;

		void addCollider(const HullCollider<dim>& collider) {
			this->emplace_back(collider);
		}
	};

	template<Dimension dim>
	DynamicHullColliders<dim> transform(const DynamicHullColliders<dim>& hullColliders, const Transform<dim>& transform) {
		DynamicHullColliders<dim> transformedHullColliders;
		transformedHullColliders.reserve(hullColliders.size());
		for(const HullCollider<dim>& hullCollider : hullColliders) {
			transformedHullColliders.emplace_back(Math::transform(hullCollider, transform));
		}
		return transformedHullColliders;
	}

}