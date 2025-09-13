#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	template<Dimension dim>
	struct CylinderCollider : public Collider<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using AABBD = AABB<dim>;
		using ColliderD = Collider<dim>;
		using TransformD = Transform<dim>;
	public:
		CylinderCollider() = default;
		CylinderCollider(PointD start, PointD end, scalar radius) : ColliderD(), m_Start(start), m_End(end), m_Radius(VectorD(radius)) { }
		CylinderCollider(PointD start, PointD end, VectorD radius) : ColliderD(), m_Start(start), m_End(end), m_Radius(radius) { }
		CylinderCollider(CylinderCollider&) = default;
		CylinderCollider(const CylinderCollider&) = default;
		~CylinderCollider() = default;

		PointD getStart() const {
			return m_Start;
		}
		PointD getEnd() const {
			return m_End;
		}
		VectorD getRadius() const {
			return m_Radius;
		}
		VectorD getAxis() const {
			return m_End - m_Start;
		}
		VectorD getHalfAxis() const {
			return getAxis() / 2.f;
		}
		PointD getCenter() const {
			return getStart() + getAxis() / 2.f;
		}

		/**
		 * @brief calculates the nearest surface point on the Cylinder to the given point
		 * @param transformToPointSpace the Cylinders Transform in the points space, probably world space -> the Cylinders World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			const VectorD axis = Math::transform(getHalfAxis(), transformToPointSpace.toMat());
			const PointD center = Math::transform(getCenter(), transformToPointSpace.toMat());
			const VectorD radius = Math::transform(getRadius(), transformToPointSpace.toMat());

			const VectorD pointCenterLocal = point - center;
			if(pointCenterLocal.isNullVector()) return point;

			const scalar pointAxisComponent = pointCenterLocal.projectionScaleOn(axis);
			const bool isPointOutsideAxis = pointAxisComponent < -1.f || pointAxisComponent > 1.f;
			const scalar pointDistanceFromAxis = (1.f - glm::abs(pointAxisComponent)) * axis.length();

			const VectorD radiusOnAxis = radius.projectOn(axis);
			const VectorD radiusOnPlane = radius - radiusOnAxis;

			const scalar pointOnRadiusOnPlaneComponent = pointCenterLocal.projectionScaleOn(radiusOnPlane);
			const bool isPointOutsideRadius = pointOnRadiusOnPlaneComponent < -1.f || pointOnRadiusOnPlaneComponent > 1.f;
			const scalar pointDistanceFromRadius = (1.f - glm::abs(pointOnRadiusOnPlaneComponent)) * radiusOnPlane.length();

			const bool snapAxis = pointDistanceFromAxis < pointDistanceFromRadius;
			const bool snapRadius = pointDistanceFromRadius < pointDistanceFromAxis;

			const VectorD axisOffset = (isPointOutsideAxis || (!isPointOutsideRadius && snapAxis)) ?
				snapToBounds(pointCenterLocal, -axis, axis) :
				clamp(pointCenterLocal, -axis, axis);
			const VectorD radiusOffset = (isPointOutsideRadius || (!isPointOutsideAxis && snapRadius)) ?
				snapToBounds(pointCenterLocal, -radiusOnPlane, radiusOnPlane) :
				clamp(pointCenterLocal, -radiusOnPlane, radiusOnPlane);

			return center + axisOffset + radiusOffset;
		}

		/**
		 * @brief calculates the nearest surface point on the Cylinder to the given direction
		 * @param transformToVectorSpace the Cylinders Transform in the points space, probably world space -> the Cylinders World Transform
		 * @param direction the direction to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			const VectorD axis = Math::transform(getHalfAxis(), transformToVectorSpace.toMat());
			const PointD center = Math::transform(getCenter(), transformToVectorSpace.toMat());
			const VectorD radius = Math::transform(getRadius(), transformToVectorSpace.toMat());

			const scalar projectionLengthOnAxis = axis.dot(direction) / axis.lengthSquared();
			PointD baseAxisPoint = center;
			if(projectionLengthOnAxis > 0.f) {
				baseAxisPoint = center + axis;
			} else if(projectionLengthOnAxis < 0.f) {
				baseAxisPoint = center - axis;
			}
			const VectorD radiusOnAxis = radius.projectOn(axis);
			const VectorD radiusOnPlane = radius - radiusOnAxis;

			const scalar projectionLengthOnPlane = radiusOnPlane.dot(direction) / radiusOnPlane.lengthSquared();
			PointD nearSurfacePoint = baseAxisPoint;
			if(projectionLengthOnPlane > 0.f) {
				nearSurfacePoint += radiusOnPlane;
			} else if(projectionLengthOnPlane < 0.f) {
				nearSurfacePoint -= radiusOnPlane;
			}
			return nearSurfacePoint;
		}

		virtual AABBD getBounds() const override {
			const VectorD axis = getHalfAxis();
			const PointD center = getCenter();
			const VectorD radius = getRadius();

			const VectorD radiusOnAxis = radius.projectOn(axis);
			const VectorD radiusOnPlane = radius - radiusOnAxis;

			return AABBD(center - axis - radiusOnPlane, center + axis + radiusOnPlane);
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::Cylinder; }
	protected:
		PointD m_Start = PointD(0.f);
		PointD m_End = PointD(0.f);
		VectorD m_Radius = VectorD();
	};

	template<Dimension dim>
	CylinderCollider<dim> transform(const CylinderCollider<dim>& collider, const Transform<dim>& transform) {
		return CylinderCollider<dim>(Math::transform(collider.getStart(), transform.toMat()), Math::transform(collider.getEnd(), transform.toMat()), Math::transform(collider.getRadius(), transform.toMat()));
	}
}