#pragma once
#include <C78E/Math/Core/Core.h>
#include "AABB.h"

namespace C78E::Math {

	/**
	 * @brief Cube, a cube in a dim dimensional space, represented by 8 points
	 * @tparam dim dimensionality of the Cube
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Cube {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		static constexpr PointsCount s_Count = 8;
		using Points = Points<dim, s_Count>;
		using Triangle = Triangle<dim>;
		using Triangles = Triangles<dim, 12>;
		using AABB = AABB<dim>;
	private:
		static const Points s_UnitCubePoints = {
			glm::vec3(-.5f, -.5f, -.5f),
			glm::vec3(+.5f, -.5f, -.5f),
			glm::vec3(+.5f, +.5f, -.5f),
			glm::vec3(-.5f, +.5f, -.5f),
			glm::vec3(-.5f, -.5f, +.5f),
			glm::vec3(+.5f, -.5f, +.5f),
			glm::vec3(+.5f, +.5f, +.5f),
			glm::vec3(-.5f, +.5f, +.5f)
		};
	public:
		static const Points& getUnitCubePoints() {
			return s_UnitCubePoints;
		};
	public:
		Cube() = default;
		Cube(const Point& p1, const Point& p2, const Point& p3, const Point& p4,
			const Point& p5, const Point& p6, const Point& p7, const Point& p8)
			: m_Points{ p1, p2, p3, p4, p5, p6, p7, p8 } { }
		Cube(const Point& p1, const Vector& v1, const Vector& v2, const Vector& v3,
			const Vector& v4, const Vector& v5, const Vector& v6, const Vector& v7)
			: m_Points{ p1, p1 + v1, p1 + v2, p1 + v3, p1 + v4, p1 + v5, p1 + v6, p1 + v7 } { }
		Cube(const Points& points) : m_Points(points) { }
		Cube(const Cube&) = default;
		~Cube() = default;

		inline const Points& getPoints() const {
			return m_Points;
		}
		inline void setPoints(const Points& points) {
			m_Points = points;
		}

		const Triangles getTriangles() const {
			return Triangles{ Triangle(m_Points[0], m_Points[1], m_Points[2]), Triangle(m_Points[0], m_Points[2], m_Points[3]),
				Triangle(m_Points[4], m_Points[5], m_Points[6]), Triangle(m_Points[4], m_Points[6], m_Points[7]),
				Triangle(m_Points[0], m_Points[1], m_Points[5]), Triangle(m_Points[0], m_Points[5], m_Points[4]),
				Triangle(m_Points[1], m_Points[2], m_Points[6]), Triangle(m_Points[1], m_Points[6], m_Points[5]),
				Triangle(m_Points[2], m_Points[3], m_Points[7]), Triangle(m_Points[2], m_Points[7], m_Points[6]),
				Triangle(m_Points[3], m_Points[0], m_Points[4]), Triangle(m_Points[3], m_Points[4], m_Points[7]) };
		}

		AABB getBoundingBox() const {
			return AABB{ m_Points[0], m_Points[1], m_Points[2], m_Points[3], m_Points[4], m_Points[5], m_Points[6], m_Points[7] };
		}

	protected:
		Points m_Points = getUnitCubePoints();
	};

	/**
	 * @brief Transforms a Cube given a TransformMatrix
	 * @tparam dim dimensionality of the Cube
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param cube the Cube to transform
	 * @param t the TransformMatrix to apply
	 * @return the transformed Cube
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	Cube<dim> transform(const Cube<dim>& cube, const mat<transforming>& t) {
		return Cube<dim>(transform(cube.getPoints(), t));
	}

}