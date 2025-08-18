#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	template<Dimension dim>
	struct CylinderCollider : public Collider<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using AABB = AABB<dim>;
		using Collider = Collider<dim>;
		using Transform = Transform<dim>;
	public:
		CylinderCollider() = default;
		CylinderCollider(Point start, Point end, scalar radius) : Collider(), m_Start(start), m_End(end), m_Radius(Vector(radius)) { }
		CylinderCollider(Point start, Point end, Vector radius) : Collider(), m_Start(start), m_End(end), m_Radius(radius) { }
		CylinderCollider(CylinderCollider&) = default;
		CylinderCollider(const CylinderCollider&) = default;
		~CylinderCollider() = default;

		Point getStart() const {
			return m_Start;
		}
		Point getEnd() const {
			return m_End;
		}
		Vector getRadius() const {
			return m_Radius;
		}
		Vector getAxis() const {
			return m_End - m_Start;
		}
		Vector getHalfAxis() const {
			return getAxis() / 2.f;
		}
		Point getCenter() const {
			return getStart() + getAxis() / 2.f;
		}

		/**
		 * @brief calculates the nearest surface point on the Cylinder to the given point
		 * @param transformToPointSpace the Cylinders Transform in the points space, probably world space -> the Cylinders World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			const Vector axis = Math::transform(getHalfAxis(), transformToPointSpace.toMat());
			const Point center = Math::transform(getCenter(), transformToPointSpace.toMat());
			const Vector radius = Math::transform(getRadius(), transformToPointSpace.toMat());

			const Vector pointCenterLocal = point - center;
			if(pointCenterLocal.isNullVector()) return point;

			const scalar pointAxisComponent = pointCenterLocal.projectionScaleOn(axis);
			const bool isPointOutsideAxis = pointAxisComponent < -1.f || pointAxisComponent > 1.f;
			const scalar pointDistanceFromAxis = (1.f - glm::abs(pointAxisComponent)) * axis.length();

			const Vector radiusOnAxis = radius.projectOn(axis);
			const Vector radiusOnPlane = radius - radiusOnAxis;

			const scalar pointOnRadiusOnPlaneComponent = pointCenterLocal.projectionScaleOn(radiusOnPlane);
			const bool isPointOutsideRadius = pointOnRadiusOnPlaneComponent < -1.f || pointOnRadiusOnPlaneComponent > 1.f;
			const scalar pointDistanceFromRadius = (1.f - glm::abs(pointOnRadiusOnPlaneComponent)) * radiusOnPlane.length();

			const bool snapAxis = pointDistanceFromAxis < pointDistanceFromRadius;
			const bool snapRadius = pointDistanceFromRadius < pointDistanceFromAxis;

			const Vector axisOffset = (isPointOutsideAxis || (!isPointOutsideRadius && snapAxis)) ?
				snapToBounds(pointCenterLocal, -axis, axis) :
				clamp(pointCenterLocal, -axis, axis);
			const Vector radiusOffset = (isPointOutsideRadius || (!isPointOutsideAxis && snapRadius)) ?
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
		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			const Vector axis = Math::transform(getHalfAxis(), transformToVectorSpace.toMat());
			const Point center = Math::transform(getCenter(), transformToVectorSpace.toMat());
			const Vector radius = Math::transform(getRadius(), transformToVectorSpace.toMat());

			const scalar projectionLengthOnAxis = axis.dot(direction) / axis.lengthSquared();
			Point baseAxisPoint = center;
			if(projectionLengthOnAxis > 0.f) {
				baseAxisPoint = center + axis;
			} else if(projectionLengthOnAxis < 0.f) {
				baseAxisPoint = center - axis;
			}
			const Vector radiusOnAxis = radius.projectOn(axis);
			const Vector radiusOnPlane = radius - radiusOnAxis;

			const scalar projectionLengthOnPlane = radiusOnPlane.dot(direction) / radiusOnPlane.lengthSquared();
			Point nearSurfacePoint = baseAxisPoint;
			if(projectionLengthOnPlane > 0.f) {
				nearSurfacePoint += radiusOnPlane;
			} else if(projectionLengthOnPlane < 0.f) {
				nearSurfacePoint -= radiusOnPlane;
			}
			return nearSurfacePoint;
		}

		virtual AABB getBounds() const override {
			const Vector axis = getHalfAxis();
			const Point center = getCenter();
			const Vector radius = getRadius();

			const Vector radiusOnAxis = radius.projectOn(axis);
			const Vector radiusOnPlane = radius - radiusOnAxis;

			return AABB(center - axis - radiusOnPlane, center + axis + radiusOnPlane);
		}

		virtual Collider::Type getType() const { return Collider::Type::Cylinder; }
	protected:
		Point m_Start = Point(0.f);
		Point m_End = Point(0.f);
		Vector m_Radius = Vector();
	};

	template<Dimension dim>
	CylinderCollider<dim> transform(const CylinderCollider<dim>& collider, const Transform<dim>& transform) {
		return CylinderCollider<dim>(Math::transform(collider.getStart(), transform.toMat()), Math::transform(collider.getEnd(), transform.toMat()), Math::transform(collider.getRadius(), transform.toMat()));
	}
}