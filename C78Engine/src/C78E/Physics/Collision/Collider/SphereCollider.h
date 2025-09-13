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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using SphereD = Sphere<dim>;
		using AABBD = AABB<dim>;
		using TransformD = Transform<dim>;
		using ColliderD = Collider<dim>;
	public:
		SphereCollider() = default;
		SphereCollider(PointD center, scalar radius) : ColliderD(), SphereD(center, radius) { }
		SphereCollider(PointD center, VectorD radius) : ColliderD(), SphereD(center, radius) { }
		SphereCollider(const SphereD& sphere) : ColliderD(), SphereD(sphere) { }
		SphereCollider(SphereCollider&) = default;
		SphereCollider(const SphereCollider&) = default;
		~SphereCollider() = default;

		virtual PointD getCenter() const {
			return SphereD::getCenter();
		}
		virtual VectorD getRadius() const {
			return SphereD::getRadius();
		}
		
		bool isPerfectSphere() const {
			return SphereD::isPerfectSphere();
		}

		/**
		 * @brief calculates the nearest surface point on the Sphere to the given point
		 * @param transformToPointSpace the Spheres Transform in the points space, probably world space -> the Spheres World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			const PointD pointSphereLocal = Math::transform(point, transformToPointSpace.toInvMat());
			const VectorD pointDirectionSphereLocal = pointSphereLocal - getCenter();
			const PointD nearSurfacePointSphereLocal = getCenter() + getRadius() * pointDirectionSphereLocal.normalize();
			const PointD nearSurfacePoint = Math::transform(nearSurfacePointSphereLocal, transformToPointSpace.toMat());
			return nearSurfacePoint;
		}


		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			const VectorD directionLocal = Math::transform(direction, transformToVectorSpace.toInvMat()); // Tranform Point to Sphere space
			const PointD nearSurfacePointLocal = getCenter() + getRadius() * directionLocal.normalize(); // apply on plane vector to support -> nsp
			const PointD nearSurfacePoint = Math::transform(nearSurfacePointLocal, transformToVectorSpace.toMat()); // Transform from Sphere space back to point space
			return nearSurfacePoint;
		}

		virtual AABBD getBounds() const override {
			return AABBD(getCenter() - getRadius(), getCenter() + getRadius());
		}

		virtual const SphereD& getSphere() const {
			return *this;
		}
		virtual SphereD& getSphere() {
			return *this;
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::Sphere; }
	protected:
		// Sphere
	};

	template<Dimension dim>
	SphereCollider<dim> transform(const SphereCollider<dim>& sphere, const Transform<dim>& transform) {
		return SphereCollider<dim>(Math::transform(sphere.getSphere(), transform.toMat()));
	}
}