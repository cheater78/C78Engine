#pragma once
#include "Line.h"

namespace C78E::Math {
	
	/**
	 * @brief Triangle, a triangle in a dim dimensional space, represented by 3 points
	 * @tparam dim dimensionality of the Triangle
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Triangle {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		static constexpr PointsCount s_Count = 3;
		using PointsD = Points<dim, s_Count>;
		using LineD = Line<dim>;
		using LinesD = Lines<dim, s_Count>;
	private:
		static const PointsD s_UnitTrianglePoints = {
			glm::vec2(-.5f, -.25f * std::sqrt(5.f)),
			glm::vec2(+.5f, -.25f * std::sqrt(5.f)),
			glm::vec2(0.0f, +.25f * std::sqrt(5.f))
		};
	public:
		static const PointsD& getUnitTrianglePoints() {
			return s_UnitTrianglePoints;
		};
	public:
		Triangle() = default;
		Triangle(const PointD& p1, const PointD& p2, const PointD& p3) : m_Points{ p1, p2, p3 } { }
		Triangle(const PointD& p1, const VectorD& v1, const VectorD& v2) : m_Points{ p1, p1 + v1, p1 + v2 } { }
		Triangle(const PointsD& points) : m_Points(points) { }
		~Triangle() = default;

		inline const PointsD& getPoints() const {
			return m_Points;
		}
		inline void setPoints(const PointsD& points) {
			m_Points = points;
		}

		inline const LinesD getLines() const {
			return LinesD{ LineD(m_Points[0], m_Points[1]), LineD(m_Points[1], m_Points[2]), LineD(m_Points[2], m_Points[0]) };
		}

		scalar getArea() const {
			const vecd a = m_Points[1] - m_Points[0];
			const vecd b = m_Points[2] - m_Points[0];
			return glm::length(glm::cross(a, b)) / 2.f;
		}

		inline bool isDegenerate() const {
			return m_Points[0] == m_Points[1] || m_Points[0] == m_Points[2] || m_Points[1] == m_Points[2];
		}

		inline bool operator==(const Triangle& other) const {
			return m_Points[0] == other.m_Points[0] && m_Points[1] == other.m_Points[1] && m_Points[2] == other.m_Points[2];
		}
		inline bool operator!=(const Triangle& other) const {
			return !(*this == other);
		}
	protected:
		PointsD m_Points = getUnitTrianglePoints();
	};

	/**
	 * @brief 
	 * @tparam dim 
	 * @tparam transforming 
	 * @param triangle 
	 * @param t 
	 * @return 
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	Triangle<dim> transform(const Triangle<dim>& triangle, const mat<transforming>& t) {
		return Triangle<dim>(transform(triangle.getPoints(), t));
	}

	/**
	 * @brief TrianglesCount, the number of Triangles in an array
	 */
	using TrianglesCount = size_t;
	
	/**
	 * @brief Triangles, an array of Triangles of the same Dimension
	 * @tparam dim the Dimension of the Triangles
	 * @tparam size the number of Triangles in the array
	 */
	template<Dimension dim, TrianglesCount size>
		requires (dim >= 2)
	struct Triangles : public std::array<Triangle<dim>, size> {
	public:
		using Count = TrianglesCount;
	public:
		Triangles() = default;
		Triangles(std::initializer_list<Triangle<dim>> list) : std::array<Triangle<dim>, size>(list) { }
		~Triangles() = default;
	};

	/**
	 * @brief Transforms a Triangles array given a TransformMatrix
	 * @tparam dim dimensionality of the Triangles
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param triangles the Triangles to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Triangles
	 */
	template<Dimension dim, TrianglesCount size, Dimension transforming>
		requires (dim >= 2)
	Triangles<dim, size> transform(const Triangles<dim, size>& triangles, const mat<transforming>& t) {
		Triangles<dim, size> transformedTriangles;
		for(TrianglesCount i = 0; i < size; i++) {
			transformedTriangles[i] = transform(triangles[i], t);
		}
		return transformedTriangles;
	}

	/**
	 * @brief DynamicTriangles, a dynamic array of Triangles of the same Dimension
	 * @tparam dim the Dimension of the Triangles
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicTriangles : public std::vector<Triangle<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicTriangles() = default;
		DynamicTriangles(std::initializer_list<Triangle<dim>> init) {
			this->resize(init.size());
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicTriangles() = default;
	};

	/**
	 * @brief Transforms a dynamic Triangles array given a TransformMatrix
	 * @tparam dim dimensionality of the Triangles
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param triangles the Triangles to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Triangles
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicTriangles<dim> transform(const DynamicTriangles<dim>& triangles, const mat<transforming>& t) {
		DynamicTriangles<dim> transformedTriangles;
		transformedTriangles.resize(triangles.size());
		for(size_t i = 0; i < triangles.size(); i++) {
			transformedTriangles[i] = transform(triangles[i], t);
		}
		return transformedTriangles;
	}

}
