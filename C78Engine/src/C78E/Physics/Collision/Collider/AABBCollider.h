#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	/**
	 * @brief AxisAlignedBoundingBoxCollider, based on AABB
	 * @tparam dim 2D -> AABB<2D> or 3D -> AABB<3D>
	 */
	template<Dimension dim>
	struct AABBCollider : public Collider<dim>, protected AABB<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Transform = Transform<dim>;
		using Collider = Collider<dim>;
		using AABB = AABB<dim>;
	public:
		AABBCollider() = default;
		AABBCollider(Point min, Point max) : Collider(), AABB(min, max) { }
		AABBCollider(const AABB& aabb) : Collider(), AABB(aabb) { }
		AABBCollider(AABBCollider&) = default;
		AABBCollider(const AABBCollider&) = default;
		~AABBCollider() = default;

		virtual Point getMin() const {
			return AABB::getMin();
		}
		virtual Point getMax() const {
			return AABB::getMax();
		}
		virtual Point getCenter() const {
			return AABB::getCenter();
		}
		virtual Vector getSize() const {
			return AABB::getSize();
		}
		virtual Vector getHalfExtent() const {
			return AABB::getHalfExtent();
		}

		/**
		 * @brief calculates the nearest surface point on the Box to the given point
		 * @param transformToPointSpace the Box' Transform in the points space, probably world space -> the Box' World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			const AABB boundsWorld = Math::transform(getBounds(), transformToPointSpace.toMat());
			const Point center = boundsWorld.getCenter();
			const Vector pointDirection = point - center;
			const Vector boxClampedPointDirection = snapToBounds(pointDirection, -boundsWorld.getHalfExtent(), boundsWorld.getHalfExtent());
			const Point nearSurfacePoint = center + boxClampedPointDirection;
			return nearSurfacePoint;
		}

		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			const AABB boundsWorld = Math::transform(getBounds(), transformToVectorSpace.toMat());
			const Point center = boundsWorld.getCenter();
			const Vector boxClampedDirection = direction.normalize() * boundsWorld.getHalfExtent();
			const Point nearSurfacePoint = center + boxClampedDirection;
			return nearSurfacePoint;
		}

		virtual AABB getBounds() const override {
			return *this;
		}

		virtual Collider::Type getType() const { return Collider::Type::AABB; }
	protected:
		// AABB
	};

	template<Dimension dim>
	AABBCollider<dim> transform(const AABBCollider<dim>& aabb, Transform<dim>& transform) {
		return AABBCollider<dim>(Math::transform(aabb.getBounds(), transform.toMat()));
	}

}