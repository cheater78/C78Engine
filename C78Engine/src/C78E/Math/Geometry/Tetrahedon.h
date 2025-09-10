#pragma once
#include "AABB.h"

namespace C78E::Math {

	/**
	 * @brief Tetrahedon, a tetrahedon in a dim dimensional space, represented by 4 points
	 * @tparam dim dimensionality of the Tetrahedon
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Tetrahedon {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		static constexpr PointsCount s_Count = 4;
		using PointsD = Points<dim, s_Count>;
		using TriangleD = Triangle<dim>;
		using TrianglesD = Triangles<dim, 4>;
		using AABBD = AABB<dim>;	
	private:
		static const PointsD s_UnitTetrahedonPoints = {
			glm::vec3(-.5f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), -.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(+.5f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), -.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(0.0f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), +.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(0.0f, +.5f * (std::sqrt(3.f) / std::sqrt(5.f)), 0.0f)
		};
	public:
		static const PointsD& getUnitTetrahedonPoints() {
			return s_UnitTetrahedonPoints;
		};
	public:
		Tetrahedon() = default;
		Tetrahedon(const PointD& p1, const PointD& p2, const PointD& p3, const PointD& p4) : m_Points{ p1, p2, p3, p4 } { }
		Tetrahedon(const PointD& p1, const VectorD& v1, const VectorD& v2, const VectorD& v3) : m_Points{ p1, p1 + v1, p1 + v2, p1 + v3 } { }
		Tetrahedon(const PointsD& points) : m_Points(points) { }
		Tetrahedon(const Tetrahedon&) = default;
		~Tetrahedon() = default;

		inline const PointsD& getPoints() const {
			return m_Points;
		}
		inline void setPoints(const PointsD& points) {
			m_Points = points;
		}

		const TrianglesD getTriangles() const {
			return TrianglesD{ TriangleD(m_Points[0], m_Points[1], m_Points[2]), TriangleD(m_Points[0], m_Points[2], m_Points[3]), TriangleD(m_Points[0], m_Points[3], m_Points[1]), TriangleD(m_Points[1], m_Points[2], m_Points[3]) };
		}

		AABBD getBoundingBox() const {
			return AABBD{ m_Points[0], m_Points[1], m_Points[2], m_Points[3] };
		}

		scalar getArea() const {
			scalar area = 0.f;
			for(const auto& triangle : getTriangles()) {
				area += triangle.getArea();
			}
			return area;
		}
		scalar getVolume() const {
			const vecd a = m_Points[1] - m_Points[0];
			const vecd b = m_Points[2] - m_Points[0];
			const vecd c = m_Points[3] - m_Points[0];
			return glm::length(Math::tripleProduct(a, b, c)) / 6.f;
		}

		inline bool isDegenerate() const {
			const VectorD a = m_Points[1] - m_Points[0];
			const VectorD b = m_Points[2] - m_Points[0];
			const VectorD c = m_Points[3] - m_Points[0];
			return a.isNullVector() || b.isNullVector() || c.isNullVector() ||
				a.isParallel(b) || a.isParallel(c) || b.isParallel(c);
		}

	protected:
		PointsD m_Points = getUnitTetrahedonPoints();
	};

	/**
	 * @brief Transforms a Tetrahedon with a TransformMatrix
	 * @tparam native the Dimension of the Tetrahedon
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param tetrahedon the Tetrahedon to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Tetrahedon
	 */
	template<Dimension native, Dimension transforming>
	Tetrahedon<native> transform(const Tetrahedon<native>& tetrahedon, const mat<transforming>& t) {
		return Tetrahedon<native>(transform(tetrahedon.getPoints(), t));
	}

	/**
	 * @brief TetrahedonsCount, the number of Tetrahedons in an array
	 */
	using TetrahedonsCount = size_t;

	/**
	 * @brief Tetrahedons, an array of Tetrahedons of the same Dimension
	 * @tparam dim the Dimension of the Tetrahedons
	 * @tparam size the number of Tetrahedons in the array
	 */
	template<Dimension dim, TetrahedonsCount size>
		requires (dim >= 2)
	struct Tetrahedons : public std::array<Tetrahedon<dim>, size> {
	public:
		using Count = TetrahedonsCount;
	public:
		Tetrahedons() = default;
		Tetrahedons(std::initializer_list<Tetrahedon<dim>> list) : std::array<Tetrahedon<dim>, size>(list) { }
		~Tetrahedons() = default;
	};

	/**
	 * @brief Transforms a Tetrahedons array given a TransformMatrix
	 * @tparam dim dimensionality of the Tetrahedons
	 * @tparam size the number of Tetrahedons in the array
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param tetrahedons the Tetrahedons to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Tetrahedons
	 */
	template<Dimension dim, TetrahedonsCount size, Dimension transforming>
		requires (dim >= 2)
	Tetrahedons<dim, size> transform(const Tetrahedons<dim, size>& tetrahedons, const mat<transforming>& t) {
		Tetrahedons<dim, size> transformedTetrahedons;
		for(TetrahedonsCount i = 0; i < size; i++) {
			transformedTetrahedons[i] = transform(tetrahedons[i], t);
		}
		return transformedTetrahedons;
	}

	/**
	 * @brief DynamicTetrahedons, dynamic array of Tetrahedons of the same Dimension
	 * @tparam dim the Dimension of the Tetrahedons
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicTetrahedons : public std::vector<Tetrahedon<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicTetrahedons() = default;
		DynamicTetrahedons(std::initializer_list<Tetrahedon<dim>> init) {
			this->resize(init.size());
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicTetrahedons() = default;
	};

	/**
	 * @brief Transforms a dynamic Tetrahedons array given a TransformMatrix
	 * @tparam dim dimensionality of the Tetrahedons
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param tetrahedons the Tetrahedons to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Tetrahedons
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicTetrahedons<dim> transform(const DynamicTetrahedons<dim>& tetrahedons, const mat<transforming>& t) {
		DynamicTetrahedons<dim> transformedTetrahedons;
		transformedTetrahedons.resize(tetrahedons.size());
		for(size_t i = 0; i < tetrahedons.size(); i++) {
			transformedTetrahedons[i] = transform(tetrahedons[i], t);
		}
		return transformedTetrahedons;
	}

}