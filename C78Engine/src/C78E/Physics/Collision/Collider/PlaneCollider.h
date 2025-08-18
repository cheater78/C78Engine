#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	/**
	 * @brief PlaneCollider, with a Normal, and support point normal * distance
	 * @tparam dim 2D -> LineCollider or 3D -> PlaneCollider
	 */
	template<Dimension dim>
	struct PlaneCollider : public Collider<dim>, public Plane<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Plane = Plane<dim>;
		using AABB = AABB<dim>;
		using Transform = Transform<dim>;
		using Collider = Collider<dim>;
	public:
		PlaneCollider() = default;
		PlaneCollider(Vector normal, scalar distance) : Collider(), Plane(normal, distance) { }
		PlaneCollider(PlaneCollider&) = default;
		PlaneCollider(const PlaneCollider&) = default;
		~PlaneCollider() = default;

		virtual Vector getNormal() const {
			return Plane::getNormal();
		}
		virtual scalar getDistance() const {
			return Plane::getDistance();
		}

		virtual void setNormal(Vector normal) {
			Plane::setNormal(normal);
		}
		virtual void setDistance(scalar distance) {
			Plane::setDistance(distance);
		}

		virtual Point getSupport() const {
			return Plane::getSupport();
		}

		/**
		 * @brief calculates the nearest surface point on the Plane to the given point
		 * @param transformToPointSpace the Planes Transform in the points space, probably world space -> the Planes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			const Vector pointPlaneLocal = Math::transform(point, transformToPointSpace.toInvMat()).getOriginVector(); // Tranform Point to Plane space
			const Vector pointOnNormalProjection = pointPlaneLocal.projectOn(getNormal()); // project Point on PlaneSupport
			const Vector vectorSupportToNearSurface = pointPlaneLocal - pointOnNormalProjection; // get the on plane vector component of point
			const Point nearSurfacePointLocal = getSupport() + vectorSupportToNearSurface; // apply on plane vector to support -> nsp
			const Point nearSurfacePoint = Math::transform(nearSurfacePointLocal, transformToPointSpace.toMat()); // Transform from Plane space back to point space
			return nearSurfacePoint;
		}

		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			return Math::transform(getSupport(), transformToVectorSpace.toMat());
		}


		virtual AABB getBounds() const override {
			return AABB(getSupport(), getSupport());
		}

		virtual Collider::Type getType() const override { return Collider::Type::Plane; }
	protected:
		// Plane
	};

	template<Dimension dim>
	PlaneCollider<dim> transform(const PlaneCollider<dim>& plane, const Transform<dim>& transform) {
		return PlaneCollider<dim>(Math::transform(plane.getPlane(), transform.toMat()));
	}

}