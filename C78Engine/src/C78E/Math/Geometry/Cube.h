#pragma once
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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		static constexpr PointsCount s_Count = 8;
		using PointsD = Points<dim, s_Count>;
		using TriangleD = Triangle<dim>;
		using TrianglesD = Triangles<dim, 12>;
		using AABBD = AABB<dim>;
	private:
		static const PointsD s_UnitCubePoints = {
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
		static const PointsD& getUnitCubePoints() {
			return s_UnitCubePoints;
		};
	public:
		Cube() = default;
		Cube(const PointD& p1, const PointD& p2, const PointD& p3, const PointD& p4,
			const PointD& p5, const PointD& p6, const PointD& p7, const PointD& p8)
			: m_Points{ p1, p2, p3, p4, p5, p6, p7, p8 } { }
		Cube(const PointD& p1, const VectorD& v1, const VectorD& v2, const VectorD& v3,
			const VectorD& v4, const VectorD& v5, const VectorD& v6, const VectorD& v7)
			: m_Points{ p1, p1 + v1, p1 + v2, p1 + v3, p1 + v4, p1 + v5, p1 + v6, p1 + v7 } { }
		Cube(const PointsD& points) : m_Points(points) { }
		Cube(const Cube&) = default;
		~Cube() = default;

		inline const PointsD& getPoints() const {
			return m_Points;
		}
		inline void setPoints(const PointsD& points) {
			m_Points = points;
		}

		const TrianglesD getTriangles() const {
			return TrianglesD{ TriangleD(m_Points[0], m_Points[1], m_Points[2]), TriangleD(m_Points[0], m_Points[2], m_Points[3]),
				TriangleD(m_Points[4], m_Points[5], m_Points[6]), TriangleD(m_Points[4], m_Points[6], m_Points[7]),
				TriangleD(m_Points[0], m_Points[1], m_Points[5]), TriangleD(m_Points[0], m_Points[5], m_Points[4]),
				TriangleD(m_Points[1], m_Points[2], m_Points[6]), TriangleD(m_Points[1], m_Points[6], m_Points[5]),
				TriangleD(m_Points[2], m_Points[3], m_Points[7]), TriangleD(m_Points[2], m_Points[7], m_Points[6]),
				TriangleD(m_Points[3], m_Points[0], m_Points[4]), TriangleD(m_Points[3], m_Points[4], m_Points[7]) };
		}

		AABBD getBoundingBox() const {
			return AABBD{ m_Points[0], m_Points[1], m_Points[2], m_Points[3], m_Points[4], m_Points[5], m_Points[6], m_Points[7] };
		}

	protected:
		PointsD m_Points = getUnitCubePoints();
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