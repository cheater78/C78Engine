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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using TransformD = Transform<dim>;
		using ColliderD = Collider<dim>;
		using AABBD = AABB<dim>;
	public:
		AABBCollider() = default;
		AABBCollider(PointD min, PointD max) : ColliderD(), AABBD(min, max) { }
		AABBCollider(const AABBD& aabb) : ColliderD(), AABBD(aabb) { }
		AABBCollider(AABBCollider&) = default;
		AABBCollider(const AABBCollider&) = default;
		~AABBCollider() = default;

		virtual PointD getMin() const {
			return AABBD::getMin();
		}
		virtual PointD getMax() const {
			return AABBD::getMax();
		}
		virtual PointD getCenter() const {
			return AABBD::getCenter();
		}
		virtual VectorD getSize() const {
			return AABBD::getSize();
		}
		virtual VectorD getHalfExtent() const {
			return AABBD::getHalfExtent();
		}

		/**
		 * @brief calculates the nearest surface point on the Box to the given point
		 * @param transformToPointSpace the Box' Transform in the points space, probably world space -> the Box' World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			const AABBD boundsWorld = Math::transform(getBounds(), transformToPointSpace.toMat());
			const PointD center = boundsWorld.getCenter();
			const VectorD pointDirection = point - center;
			const VectorD boxClampedPointDirection = snapToBounds(pointDirection, -boundsWorld.getHalfExtent(), boundsWorld.getHalfExtent());
			const PointD nearSurfacePoint = center + boxClampedPointDirection;
			return nearSurfacePoint;
		}

		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			const AABBD boundsWorld = Math::transform(getBounds(), transformToVectorSpace.toMat());
			const PointD center = boundsWorld.getCenter();
			const VectorD boxClampedDirection = direction.normalize() * boundsWorld.getHalfExtent();
			const PointD nearSurfacePoint = center + boxClampedDirection;
			return nearSurfacePoint;
		}

		virtual AABBD getBounds() const override {
			return *this;
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::AABB; }
	protected:
		// AABB
	};

	template<Dimension dim>
	AABBCollider<dim> transform(const AABBCollider<dim>& aabb, Transform<dim>& transform) {
		return AABBCollider<dim>(Math::transform(aabb.getBounds(), transform.toMat()));
	}

}