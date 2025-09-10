#pragma once
#include "Triangle.h"

namespace C78E::Math {

	/**
	 * @brief Quad, a quad in a dim dimensional space, represented by 4 points
	 * @tparam dim dimensionality of the Quad
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Quad {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		static constexpr PointsCount s_Count = 4;
		using PointsD = Points<dim, s_Count>;
		using LineD = Line<dim>;
		using LinesD = Lines<dim, 4>;
		using TriangleD = Triangle<dim>;
		using TrianglesD = Triangles<dim, 2>;
	private:
		inline static const PointsD s_UnitQuadPoints{
			glm::vec2(-.5f, -.5f),
			glm::vec2(+.5f, -.5f),
			glm::vec2(+.5f, +.5f),
			glm::vec2(-.5f, +.5f)
		};
	public:
		static const PointsD& getUnitQuadPoints() {
			return s_UnitQuadPoints;
		};
	public:
		Quad() = default;
		Quad(const PointD& p1, const PointD& p2, const PointD& p3, const PointD& p4) : m_Points{ p1, p2, p3, p4 } { }
		Quad(const PointD& p1, const VectorD& v1, const VectorD& v2, const VectorD& v3) : m_Points{ p1, p1 + v1, p1 + v2, p1 + v3 } { }
		Quad(const PointsD& points) : m_Points(points) { }
		Quad(const Quad&) = default;
		~Quad() = default;

		const PointsD& getPoints() const {
			return m_Points;
		}
		void setPoints(const PointsD& points) {
			m_Points = points;
		}

		const LinesD getLines() const {
			return LinesD{ LineD(m_Points[0], m_Points[1]), LineD(m_Points[1], m_Points[2]), LineD(m_Points[2], m_Points[3]), LineD(m_Points[3], m_Points[0]) };
		}

		const TrianglesD getTriangles() const {
			return TrianglesD{ TriangleD(m_Points[0], m_Points[1], m_Points[2]), TriangleD(m_Points[0], m_Points[2], m_Points[3]) };
		}

		scalar getArea() const {
			const vecd a = m_Points[1] - m_Points[0];
			const vecd b = m_Points[2] - m_Points[0];
			return glm::length(glm::cross(a, b));
		}

		inline bool isDegenerate() const {
			return m_Points[0] == m_Points[1] || m_Points[0] == m_Points[2] || m_Points[0] == m_Points[3] ||
				m_Points[1] == m_Points[2] || m_Points[1] == m_Points[3] || m_Points[2] == m_Points[3];
		}

		inline bool operator==(const Quad& other) const {
			return m_Points[0] == other.m_Points[0] && m_Points[1] == other.m_Points[1] &&
				m_Points[2] == other.m_Points[2] && m_Points[3] == other.m_Points[3];
		}
		inline bool operator!=(const Quad& other) const {
			return !(*this == other);
		}
	protected:
		PointsD m_Points = getUnitQuadPoints();
	};

	/**
	 * @brief Transforms a Quad given a TransformMatrix
	 * @tparam dim dimensionality of the Quad
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param quad the Quad to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Quad
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	Quad<dim> transform(const Quad<dim>& quad, const mat<transforming>& t) {
		return Quad<dim>(transform(quad.getPoints(), t));
	}

	using QuadCount = size_t;
	/**
	 * @brief Quads, an array of Quads of the same Dimension
	 * @tparam dim the Dimension of the Quads
	 * @tparam size the number of Quads in the array
	 */
	template<Dimension dim, QuadCount size>
		requires (dim >= 2)
	struct Quads : public std::array<Quad<dim>, size> {
	public:
		using Count = QuadCount;
	public:
		Quads() = default;
		Quads(std::initializer_list<Quad<dim>> list) : std::array<Quad<dim>, size>(list) { }
		~Quads() = default;
	};

	/**
	 * @brief Transforms a Quads array given a TransformMatrix
	 * @tparam dim dimensionality of the Quads
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param quads the Quads to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Quads
	 */
	template<Dimension dim, QuadCount size, Dimension transforming>
		requires (dim >= 2)
	Quads<dim, size> transform(const Quads<dim, size>& quads, const mat<transforming>& t) {
		Quads<dim, size> transformedQuads;
		for(QuadCount i = 0; i < size; i++) {
			transformedQuads[i] = transform(quads[i], t);
		}
		return transformedQuads;
	}

	/**
	 * @brief DynamicQuads, dynamic array of Quads of the same Dimension
	 * @tparam dim the Dimension of the Quads
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicQuads : public std::vector<Quad<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicQuads() = default;
		DynamicQuads(std::initializer_list<Quad<dim>> init) {
			this->resize(init.size());
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicQuads() = default;
	};

	/**
	 * @brief Transforms a dynamic Quads array given a TransformMatrix
	 * @tparam dim dimensionality of the Quads
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param quads the Quads to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Quads
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicQuads<dim> transform(const DynamicQuads<dim>& quads, const mat<transforming>& t) {
		DynamicQuads<dim> transformedQuads;
		transformedQuads.resize(quads.size());
		for(size_t i = 0; i < quads.size(); i++) {
			transformedQuads[i] = transform(quads[i], t);
		}
		return transformedQuads;
	}

}