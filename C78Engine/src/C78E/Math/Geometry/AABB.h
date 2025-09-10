#pragma once
#include "Quad.h"

namespace C78E::Math {

	/**
	 * @brief Axis Aligned Bounding Box
	 * @tparam dim 2D or 3D
	 */
	template<Dimension dim>
	struct AABB {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		template<PointsCount count>
		using PointsD = Points<dim, count>;
		template<VectorsCount count>
		using VectorsD = Vectors<dim, count>;
	public:
		AABB() = default;
		AABB(const PointD& min) : m_Min(min), m_Max(min) { } // AABB with same min and max
		AABB(const PointD& c0, const PointD& c1) : m_Min(min<dim>(c0, c1)), m_Max(max<dim>(c0, c1)) { }
		AABB(const std::initializer_list<PointD>& points) : m_Min(+scalar_limit::infinity()), m_Max(-scalar_limit::infinity()) {
			C78E_CORE_ASSERT(points.size() > 1, "AABB: initializer list size must be atleast 2!");
			for(const auto& point : points) {
				growToInclude(point);
			}
		}
		AABB(const PointsD<1 << dim>& points) : m_Min(+scalar_limit::infinity()), m_Max(-scalar_limit::infinity()) {
			for(PointsCount i = 0; i < (1 << dim); i++) {
				growToInclude(points[i]);
			}
		}
		AABB(AABB&) = default;
		AABB(const AABB&) = default;
		~AABB() = default;

		void reset() {
			m_Min = PointD(+scalar_limit::infinity());
			m_Max = PointD(-scalar_limit::infinity());
		}

		void growToInclude(PointD point) {
			m_Min = Math::min<dim>(m_Min, point);
			m_Max = Math::max<dim>(m_Max, point);
		}
		void growToInclude(AABB<dim> box) {
			m_Min = glm::min(m_Min, box.getMin());
			m_Max = glm::max(m_Max, box.getMax());
		}
		void growToInlude(const PointsD<1 << dim>& points) {
			for(PointsCount i = 0; i < (1 << dim); i++) {
				growToInclude(points[i]);
			}
		}

		PointD getMin() const {
			return m_Min;
		}
		PointD getMax() const {
			return m_Max;
		}

		PointD getCenter() const {
			return PointD(((vecd)m_Min + (vecd)m_Max) * 0.5f);
		}
		VectorD getSize() const {
			return (m_Max - m_Min).abs();
		}
		VectorD getHalfExtent() const {
			return getSize() * 0.5f;
		}

		PointsD<1 << dim> getPoints() const {
			constexpr size_t vertexCount = 1 << dim; // 2^dim vertices
			const vecd boxExtent = getHalfExtent();

			PointsD<vertexCount> vertices;
			for(int i = 0; i < vertexCount; i++) { // construct 2^dim vertices
				vertices[i] = PointD(0.f); // clear vertex to 0.f
				for(int j = 0; j < dim; j++) { // i represents all permutations in its bits
					vertices[i][j] = (i & (1 << j)) ? +boxExtent[j] : -boxExtent[j]; // bit i[dim] ? pos extent : neg extent
				}
			}
			return vertices;
		}

		VectorsD<dim> getNormals() const {
			VectorsD<dim> normals;
			for(int i = 0; i < dim; i++) { // construct dim normals
				normals[i] = VectorD(); // clear normal to 0.f
				normals[i][i] = 1.f; // one normal for each dim
			}
			return normals;
		}

		bool operator==(const AABB& other) const {
			return m_Min == other.m_Min && m_Max == other.m_Max;
		}


	protected:
		PointD m_Min = PointD(+scalar_limit::infinity());
		PointD m_Max = PointD(-scalar_limit::infinity());
	};

	/**
	 * @brief Transforms an AABB with a TransformMatrix
	 * @tparam dim dimensionality of the AABB
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param aabb the AABB to transform
	 * @param translation the translation to apply
	 * @param scale the scale to apply
	 * @return the transformed AABB
	 */
	template<Dimension dim>
	requires (dim > 1)
	AABB<dim> transform(const AABB<dim>& aabb, const Point<dim>& translation, const Vector<dim>& scale) {
		return AABB<dim>(aabb.getMin().getOriginVector() * scale + translation, aabb.getMax().getOriginVector() * scale + translation);
	}

	/**
	 * @brief Transforms an AABB with a TransformMatrix
	 * @tparam dim dimensionality of the AABB
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param aabb the AABB to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed AABB
	 */
	template<Dimension dim, Dimension transforming>
	AABB<dim> transform(const AABB<dim>& aabb, const mat<transforming>& t) {
		const Points<dim, 1 << dim> vertices = transform(aabb.getPoints(), t);
		return AABB<dim>(vertices);
	}

	template<Dimension dim>
	using AABBAABBIntersection = Intersection<dim, AABB<dim>>;

	template<Dimension dim>
	AABBAABBIntersection<dim> intersection(const AABB<dim>& boxA, const AABB<dim>& boxB) {
		if(boxA == boxB) {
			return AABBAABBIntersection<dim>::createIdentical();
		}
		
		const vec<dim> aMin = boxA.getMin().getPosition();
		const vec<dim> aMax = boxA.getMax().getPosition();
		const vec<dim> bMin = boxB.getMin().getPosition();
		const vec<dim> bMax = boxB.getMax().getPosition();

		const auto vLEminAmaxB = glm::lessThanEqual(aMin, bMax);
		const auto vLEminBmaxA = glm::lessThanEqual(bMin, aMax);
		const auto vLminAmaxB = glm::lessThan(aMin, bMax);
		const auto vLminBmaxA = glm::lessThan(bMin, aMax);

		const auto vLEminAminB = glm::lessThanEqual(aMax, bMax);
		const auto vLEmaxAmaxB = glm::lessThanEqual(aMin, bMin);
		const auto vGEminAminB = glm::greaterThanEqual(aMax, bMax);
		const auto vGEmaxAmaxB = glm::greaterThanEqual(aMin, bMin);

		const auto vLEAB = vLEminAmaxB && vLEminBmaxA; // true if axis is touching or overlapping
		const auto vLAB = vLminAmaxB && vLminBmaxA; // true if axis is overlapping

		const bool hasIntersection = glm::all(vLEAB);

		const bool touching = glm::any(vLEAB) && !glm::all(vLAB);
		const bool aContainsB = glm::all(vLEminAminB) && glm::all(vGEmaxAmaxB);
		const bool aInsideB = glm::all(vGEminAminB) && glm::all(vLEmaxAmaxB);

		if(hasIntersection) {
			AABB<dim> intersection(elementWiseMax(aMin, bMin), elementWiseMin(aMax, bMax));
			if(touching) {
				return AABBAABBIntersection<dim>::createTouching(intersection);
			}
			if(aContainsB) {
				return AABBAABBIntersection<dim>::createContaining();
			}
			if(aInsideB) {
				return AABBAABBIntersection<dim>::createInside();
			}
			return AABBAABBIntersection<dim>::createIntersection(intersection);
		}

		// No Intersection
		return AABBAABBIntersection<dim>::createNone();
	}

}