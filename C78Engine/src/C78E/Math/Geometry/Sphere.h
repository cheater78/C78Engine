#pragma once
#include <C78E/Math/Math.h>
#include "AABB.h"

namespace C78E::Math {

	/**
	 * @brief Sphere, a sphere in a dim dimensional space, represented by a center point and a radius
	 * @tparam dim dimensionality of the Sphere
	 */
	template<Dimension dim>
	struct Sphere {
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using AABB = AABB<dim>;
	public:
		static Sphere getUnitSphere() { return Sphere(Point(0.f), Vector(1.f)); }
	public:
		Sphere() = default;
		Sphere(Point center, scalar radius) : m_Center(center), m_Radius(Vector(radius)) { }
		Sphere(Point center, Vector radius = Vector()) : m_Center(center), m_Radius(radius) { }
		Sphere(Sphere&) = default;
		Sphere(const Sphere&) = default;
		~Sphere() = default;

		void setCenter(const Point& center) {
			m_Center = center;
		}
		void setRadius(const Vector& radius) {
			m_Radius = radius;
		}

		Point getCenter() const {
			return m_Center;
		}
		Vector getRadius() const {
			return m_Radius;
		}

		AABB getBoundingBox() const {
			return AABB(m_Center - m_Radius, m_Center + m_Radius);
		}

		bool isPerfectSphere() const {
			const scalar first = m_Radius[0];
			for(Dimension i = 1; i < dim; i++) {
				if(m_Radius[i] != first)
					return false;
			}
			return true;
		}

		bool isDegenerate() const {
			return m_Radius == 0.f;
		}
		operator bool() const {
			return !isDegenerate();
		}
		bool operator==(const Sphere& other) const {
			return m_Center == other.m_Center && m_Radius == other.m_Radius;
		}
	protected:
		Point m_Center = vecd(0.f);
		Vector m_Radius = 0.f;
	};

	/**
	 * @brief Transforms a Sphere with a TransformMatrix
	 * @tparam native the Dimension of the Sphere
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param sphere the Sphere to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Sphere
	 */
	template<Dimension native, Dimension transforming>
	Sphere<native> transform(const Sphere<native>& sphere, const mat<transforming>& t) {
		const mat<native + 1> nativeTransform = Math::refitTransformMatrix<transforming, native + 1>(t);

		const Point<native> center = sphere.getCenter();
		const Vector<native> radius = sphere.getRadius();
		const Point<native> transformedCenter = Math::transform(center, nativeTransform);
		const Vector<native> transformedRadius = Math::transform(radius, nativeTransform);

		return Sphere<native>(transformedCenter, transformedRadius);
	}

	/**
	 * @brief SpheresCount, the number of Spheres in an array
	 */
	using SpheresCount = size_t;

	/**
	 * @brief Spheres, an array of Spheres of the same Dimension
	 * @tparam dim the Dimension of the Spheres
	 * @tparam size the number of Spheres in the array
	 */
	template<Dimension dim, SpheresCount size>
		requires (dim >= 2)
	struct Spheres : public std::array<Sphere<dim>, size> {
	public:
		using Count = SpheresCount;
	public:
		Spheres() = default;
		Spheres(std::initializer_list<Sphere<dim>> list) : std::array<Sphere<dim>, size>(list) { }
		~Spheres() = default;
	};

	/**
	 * @brief Transforms a Spheres array given a TransformMatrix
	 * @tparam dim dimensionality of the Spheres
	 * @tparam size the number of Spheres in the array
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param spheres the Spheres to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Spheres
	 */
	template<Dimension dim, SpheresCount size, Dimension transforming>
		requires (dim >= 2)
	Spheres<dim, size> transform(const Spheres<dim, size>& spheres, const mat<transforming>& t) {
		Spheres<dim, size> transformedSpheres;
		for(SpheresCount i = 0; i < size; i++) {
			transformedSpheres[i] = transform(spheres[i], t);
		}
		return transformedSpheres;
	}

	/**
	 * @brief DynamicSpheres, dynamic array of Spheres of the same Dimension
	 * @tparam dim the Dimension of the Spheres
	 */
	template<Dimension dim>
		requires (dim >= 2)
	struct DynamicSpheres : public std::vector<Sphere<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicSpheres() = default;
		DynamicSpheres(std::initializer_list<Sphere<dim>> init) {
			this->resize(init.size());
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicSpheres() = default;
	};

	/**
	 * @brief Transforms a dynamic Spheres array given a TransformMatrix
	 * @tparam dim dimensionality of the Spheres
	 * @tparam transforming dimensionality of the TransformMatrix
	 * @param spheres the Spheres to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Spheres
	 */
	template<Dimension dim, Dimension transforming>
		requires (dim >= 2)
	DynamicSpheres<dim> transform(const DynamicSpheres<dim>& spheres, const mat<transforming>& t) {
		DynamicSpheres<dim> transformedSpheres;
		transformedSpheres.resize(spheres.size());
		for(size_t i = 0; i < spheres.size(); i++) {
			transformedSpheres[i] = transform(spheres[i], t);
		}
		return transformedSpheres;
	}

}