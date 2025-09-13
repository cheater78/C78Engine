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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using PlaneD = Plane<dim>;
		using AABBD = AABB<dim>;
		using TransformD = Transform<dim>;
		using ColliderD = Collider<dim>;
	public:
		PlaneCollider() = default;
		PlaneCollider(VectorD normal, scalar distance) : ColliderD(), PlaneD(normal, distance) { }
		PlaneCollider(PlaneCollider&) = default;
		PlaneCollider(const PlaneCollider&) = default;
		~PlaneCollider() = default;

		virtual VectorD getNormal() const {
			return PlaneD::getNormal();
		}
		virtual scalar getDistance() const {
			return PlaneD::getDistance();
		}

		virtual void setNormal(VectorD normal) {
			PlaneD::setNormal(normal);
		}
		virtual void setDistance(scalar distance) {
			PlaneD::setDistance(distance);
		}

		virtual PointD getSupport() const {
			return PlaneD::getSupport();
		}

		/**
		 * @brief calculates the nearest surface point on the Plane to the given point
		 * @param transformToPointSpace the Planes Transform in the points space, probably world space -> the Planes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			const VectorD pointPlaneLocal = Math::transform(point, transformToPointSpace.toInvMat()).getOriginVector(); // Tranform Point to Plane space
			const VectorD pointOnNormalProjection = pointPlaneLocal.projectOn(getNormal()); // project Point on PlaneSupport
			const VectorD vectorSupportToNearSurface = pointPlaneLocal - pointOnNormalProjection; // get the on plane vector component of point
			const PointD nearSurfacePointLocal = getSupport() + vectorSupportToNearSurface; // apply on plane vector to support -> nsp
			const PointD nearSurfacePoint = Math::transform(nearSurfacePointLocal, transformToPointSpace.toMat()); // Transform from Plane space back to point space
			return nearSurfacePoint;
		}

		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			return Math::transform(getSupport(), transformToVectorSpace.toMat());
		}


		virtual AABBD getBounds() const override {
			return AABBD(getSupport(), getSupport());
		}

		virtual ColliderD::Type getType() const override { return ColliderD::Type::Plane; }
	protected:
		// Plane
	};

	template<Dimension dim>
	PlaneCollider<dim> transform(const PlaneCollider<dim>& plane, const Transform<dim>& transform) {
		return PlaneCollider<dim>(Math::transform(plane.getPlane(), transform.toMat()));
	}

}