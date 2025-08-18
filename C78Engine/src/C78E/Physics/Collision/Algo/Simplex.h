#pragma once
#include <C78E/Math/Math.h>
#include <C78E/Core/Log/SmartLog.h>

namespace C78E::Physics {

	template<Dimension dim>
	struct Simplex {
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		template<PointsCount size>
		using Points = Points<dim, size>;
		template<VectorsCount size>
		using Vectors = Vectors<dim, size>;
	public:
		Simplex() : m_Points(), m_Size(0) { }

		Simplex& operator=(std::initializer_list<Point> list) {
			for(auto vit = list.begin(); vit != list.end(); vit++) {
				m_Points[std::distance(list.begin(), vit)] = *vit;
			}
			m_Size = list.size();
			return *this;
		}

		void push_front(const Point& point) {
			const size_t lastElem = std::min<size_t>(m_Size, dim);
			m_Size = lastElem + 1;
			for(size_t i = lastElem; i != 0; i--) {
				m_Points[i] = m_Points[i - 1];
			}
			m_Points[0] = point;
		}

		void remove(size_t index) {
			if(index >= m_Size) {
				return; // Or assert / throw, depending on your safety policy
			}

			for(size_t i = index; i < m_Size - 1; ++i) {
				m_Points[i] = m_Points[i + 1];
			}

			--m_Size;
		}

		Point& operator[](size_t i) { return m_Points[i]; }
		const Point& operator[](size_t i) const { return m_Points[i]; }
		size_t size() const { return m_Size; }

		auto begin() const { return m_Points.begin(); }
		auto end()   const { return m_Points.end() - (dim + 1u - m_Size); }
	private:
		Points<dim + 1> m_Points;
		size_t m_Size;
	};

}