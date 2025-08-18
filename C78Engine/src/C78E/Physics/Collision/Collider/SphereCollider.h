#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	/**
	 * @brief SphereCollider, with a Center and Radius
	 * @tparam dim 2D -> CircleCollider or 3D -> SphereCollider
	 */
	template<Dimension dim>
	struct SphereCollider : public Collider<dim>, protected Sphere<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Sphere = Sphere<dim>;
		using AABB = AABB<dim>;
		using Transform = Transform<dim>;
		using Collider = Collider<dim>;
	public:
		SphereCollider() = default;
		SphereCollider(Point center, scalar radius) : Collider(), Sphere(center, radius) { }
		SphereCollider(Point center, Vector radius) : Collider(), Sphere(center, radius) { }
		SphereCollider(const Sphere& sphere) : Collider(), Sphere(sphere) { }
		SphereCollider(SphereCollider&) = default;
		SphereCollider(const SphereCollider&) = default;
		~SphereCollider() = default;

		virtual Point getCenter() const {
			return Sphere::getCenter();
		}
		virtual Vector getRadius() const {
			return Sphere::getRadius();
		}
		
		bool isPerfectSphere() const {
			return Sphere::isPerfectSphere();
		}

		/**
		 * @brief calculates the nearest surface point on the Sphere to the given point
		 * @param transformToPointSpace the Spheres Transform in the points space, probably world space -> the Spheres World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			const Point pointSphereLocal = Math::transform(point, transformToPointSpace.toInvMat());
			const Vector pointDirectionSphereLocal = pointSphereLocal - getCenter();
			const Point nearSurfacePointSphereLocal = getCenter() + getRadius() * pointDirectionSphereLocal.normalize();
			const Point nearSurfacePoint = Math::transform(nearSurfacePointSphereLocal, transformToPointSpace.toMat());
			return nearSurfacePoint;
		}


		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			const Vector directionLocal = Math::transform(direction, transformToVectorSpace.toInvMat()); // Tranform Point to Sphere space
			const Point nearSurfacePointLocal = getCenter() + getRadius() * directionLocal.normalize(); // apply on plane vector to support -> nsp
			const Point nearSurfacePoint = Math::transform(nearSurfacePointLocal, transformToVectorSpace.toMat()); // Transform from Sphere space back to point space
			return nearSurfacePoint;
		}

		virtual AABB getBounds() const override {
			return AABB(getCenter() - getRadius(), getCenter() + getRadius());
		}

		virtual const Sphere& getSphere() const {
			return *this;
		}
		virtual Sphere& getSphere() {
			return *this;
		}

		virtual Collider::Type getType() const { return Collider::Type::Sphere; }
	protected:
		// Sphere
	};

	template<Dimension dim>
	SphereCollider<dim> transform(const SphereCollider<dim>& sphere, const Transform<dim>& transform) {
		return SphereCollider<dim>(Math::transform(sphere.getSphere(), transform.toMat()));
	}
}