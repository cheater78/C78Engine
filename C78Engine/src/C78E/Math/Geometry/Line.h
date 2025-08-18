#pragma once
#include "Point.h"
#include "Intersection.h"

namespace C78E::Math {

	/**
	 * @brief Line, a line in a dim dimensional space, represented by a direction and a support point
	 * @tparam dim dimensionality of the Line
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Line {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
	public:
		Line() = default;
		Line(const Vector& direction, const Point& support = Point(0.f)) : m_Direction(direction.normalize()), m_Support(support) { }
		Line(const Point& support1, const Point& support2) : m_Support(support1), m_Direction((support2 - support1).normalize()) { }
		Line(Line&) = default;
		Line(const Line&) = default;
		~Line() = default;

		inline void setSupport(Point support) {
			m_Support = support;
		}
		inline void setDirection(Vector direction) {
			m_Direction = direction.normalize();
		}

		inline Point getSupport() const {
			return m_Support;
		}
		inline Vector getDirection() const {
			return m_Direction;
		}
		
		inline bool isDegenerate() const {
			return m_Direction.isNullVector();
		}
		inline operator bool() const {
			return !isDegenerate();
		}
		bool operator==(const Line& other) const {
			if(m_Direction != other.m_Direction)
				return false;
			if(isDegenerate() || other.isDegenerate())
				return isDegenerate() && other.isDegenerate();
			const vecd supportDiff = other.m_Support - m_Support; // stop playing League and go outside

			scalar scale = supportDiff[0] / m_Direction[0];
			for(uint8_t i = 1; i < dim; i++) {
				if(supportDiff[dim] / m_Direction[dim] != scale)
					return false;
			}
			return true;
		}
	private:
		Point m_Support = Point(0.f);
		Vector m_Direction = Vector(0.f);
	};

	/**
	 * @brief Transforms a Line given a TransformMatrix
	 * @tparam dim dimensionality of the Line
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param line the Line to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Line
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	Line<dim> transform(const Line<dim>& line, const mat<transforming>& t) {
		return Line<dim>(transformVector<dim, transforming>(line.getDirection(), t), transformPosition<dim, transforming>(line.getSupport(), t));
	}

	template<Dimension dim>
	using LineLineIntersection = Intersection<dim, Point<dim>>;

	template<Dimension dim>
		requires (dim >= 2)
	LineLineIntersection<dim> intersect(const Line<dim>& line1, const Line<dim>& line2) {
		const Point<dim> p1 = line1.getSupport();
		const Point<dim> p2 = line2.getSupport();
		const Vector<dim> d1 = line1.getDirection();
		const Vector<dim> d2 = line2.getDirection();
		const scalar denom = d1.dot(d2);
		if(isNearZero(denom)) {
			if(line1 == line2) {
				return LineLineIntersection<dim>::createIdentical();
			} else {
				return LineLineIntersection<dim>::createNone();
			}
		}
		const scalar t = (p2 - p1).dot(d2) / denom;
		return LineLineIntersection<dim>::createIntersection(p1 + t * d1);
	}

	/**
	 * @brief LinesCount, the type used to count Lines
	 */
	using LinesCount = size_t;

	/**
	 * @brief Lines, an array of Lines of the same Dimension
	 * @tparam dim the Dimension of the Lines
	 * @tparam size the number of Lines in the array
	 */
	template<Dimension dim, LinesCount size>
		requires (dim >= 2)
	struct Lines : std::array<Line<dim>, size> {
	public:
		Lines() = default;
		Lines(std::initializer_list<Line<dim>> list) : std::array<Line<dim>, size>(list) { }
		~Lines() = default;
	};

	/**
	 * @brief Transforms a Lines array given a TransformMatrix
	 * @tparam dim dimensionality of the Lines
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param lines the Lines to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Lines
	 */
	template<Dimension dim, LinesCount size, Dimension transforming>
		requires (dim >= 2)
	Lines<dim, size> transform(const Lines<dim, size>& lines, const mat<transforming>& t) {
		Lines<dim, size> transformedLines;
		for(LinesCount i = 0; i < size; i++) {
			transformedLines[i] = transform(lines[i], t);
		}
		return transformedLines;
	}

	/**
	 * @brief DynamicLines, dynamic array of Lines of the same Dimension
	 * @tparam dim the Dimension of the Lines
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicLines : public std::vector<Line<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicLines() = default;
		DynamicLines(std::initializer_list<Line<dim>> init) {
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicLines() = default;
	};

	/**
	 * @brief Transforms a dynamic Lines array given a TransformMatrix
	 * @tparam dim dimensionality of the Lines
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param lines the Lines to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Lines
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicLines<dim> transform(const DynamicLines<dim>& lines, const mat<transforming>& t) {
		DynamicLines<dim> transformedLines;
		transformedLines.resize(lines.size());
		for(size_t i = 0; i < lines.size(); i++) {
			transformedLines[i] = transform(lines[i], t);
		}
		return transformedLines;
	}

}