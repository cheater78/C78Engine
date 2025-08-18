#pragma once
#include <C78E/Math/Core/Core.h>
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
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		static constexpr PointsCount s_Count = 4;
		using Points = Points<dim, s_Count>;
		using Triangle = Triangle<dim>;
		using Triangles = Triangles<dim, 4>;
		using AABB = AABB<dim>;	
	private:
		static const Points s_UnitTetrahedonPoints = {
			glm::vec3(-.5f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), -.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(+.5f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), -.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(0.0f, -.5f * (std::sqrt(3.f) / std::sqrt(5.f)), +.5f * (std::sqrt(3.f) / std::sqrt(5.f))),
			glm::vec3(0.0f, +.5f * (std::sqrt(3.f) / std::sqrt(5.f)), 0.0f)
		};
	public:
		static const Points& getUnitTetrahedonPoints() {
			return s_UnitTetrahedonPoints;
		};
	public:
		Tetrahedon() = default;
		Tetrahedon(const Point& p1, const Point& p2, const Point& p3, const Point& p4) : m_Points{ p1, p2, p3, p4 } { }
		Tetrahedon(const Point& p1, const Vector& v1, const Vector& v2, const Vector& v3) : m_Points{ p1, p1 + v1, p1 + v2, p1 + v3 } { }
		Tetrahedon(const Points& points) : m_Points(points) { }
		Tetrahedon(const Tetrahedon&) = default;
		~Tetrahedon() = default;

		inline const Points& getPoints() const {
			return m_Points;
		}
		inline void setPoints(const Points& points) {
			m_Points = points;
		}

		const Triangles getTriangles() const {
			return Triangles{ Triangle(m_Points[0], m_Points[1], m_Points[2]), Triangle(m_Points[0], m_Points[2], m_Points[3]), Triangle(m_Points[0], m_Points[3], m_Points[1]), Triangle(m_Points[1], m_Points[2], m_Points[3]) };
		}

		AABB getBoundingBox() const {
			return AABB{ m_Points[0], m_Points[1], m_Points[2], m_Points[3] };
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
			const Vector a = m_Points[1] - m_Points[0];
			const Vector b = m_Points[2] - m_Points[0];
			const Vector c = m_Points[3] - m_Points[0];
			return a.isNullVector() || b.isNullVector() || c.isNullVector() ||
				a.isParallel(b) || a.isParallel(c) || b.isParallel(c);
		}

	protected:
		Points m_Points = getUnitTetrahedonPoints();
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