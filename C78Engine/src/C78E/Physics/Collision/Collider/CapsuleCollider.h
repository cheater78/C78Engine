#pragma once
#include <C78E/Math/Math.h>
#include "AbstractCollider.h"

namespace C78E::Physics {

	/**
	 * @brief CapsuleCollider, with a base LineSegment form start to end, extending of by radius
	 * @tparam dim 2D or 3D 
	 */
	template<Dimension dim>
	struct CapsuleCollider : public Collider<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using AABB = AABB<dim>;
		using Collider = Collider<dim>;
		using Transform = Transform<dim>;
	public:
		CapsuleCollider() = default;
		CapsuleCollider(Point start, Point end, scalar radius) : Collider(), m_Start(start), m_End(end), m_Radius(Vector(radius)) { }
		CapsuleCollider(Point start, Point end, Vector radius) : Collider(), m_Start(start), m_End(end), m_Radius(radius) { }
		CapsuleCollider(CapsuleCollider&) = default;
		CapsuleCollider(const CapsuleCollider&) = default;
		~CapsuleCollider() = default;

		Point getStart() const {
			return m_Start;
		}
		Point getEnd() const {
			return m_End;
		}
		Vector getRadius() const { // HalfExtent
			return m_Radius;
		}
		Vector getAxis() const {
			return m_End - m_Start;
		}
		Point getCenter() const {
			return (getStart() + getEnd()) * .5f;
		}

		/**
		 * @brief calculates the nearest surface point on the Capsule to the given point
		 * @param transformToPointSpace the Capsules Transform in the points space, probably world space -> the Capsules World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			const matd transCapsuleLocalToPointSpace = transformToPointSpace.toMat();
			const matd transPointSpaceToCapsuleLocal = transformToPointSpace.toInvMat();

			const Point pointCapsuleLocal = Math::transform(point, transPointSpaceToCapsuleLocal);

			// Axis from Start to End, check startLocal point vector -> axis relative length projection of point on axis (,0] (0,1) [1,)
			const scalar pointAxisComponent = getAxis().dot(pointCapsuleLocal - getStart()) / getAxis().lengthSquared();

			const Point baseAxisPointCapsuleLocal = getStart() + glm::clamp(pointAxisComponent, 0.f, 1.f) * getAxis();
			const Vector normalBaseAxisPointToPointDirectionCapsuleLocal = (pointCapsuleLocal - baseAxisPointCapsuleLocal).getDirection();

			const Point nearSurfacePointCapsuleLocal = baseAxisPointCapsuleLocal + ellipsiodSurfacePointOffset(normalBaseAxisPointToPointDirectionCapsuleLocal);
			const Point nearSurfacePoint = Math::transform(nearSurfacePointCapsuleLocal, transCapsuleLocalToPointSpace);
			return nearSurfacePoint;
		}

		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			const matd transCapsuleLocalToVectorSpace = transformToVectorSpace.toMat();
			const matd transVectorSpaceToCapsuleLocal = transformToVectorSpace.toInvMat();

			const Vector directionCapsuleLocal = Math::transform(direction, transVectorSpaceToCapsuleLocal);

			// Axis from Start to End, check startLocal point vector -> axis relative length projection of point on axis (,0] (0,1) [1,)
			const scalar directionAxisComponent = getAxis().dot(directionCapsuleLocal) / getAxis().lengthSquared();

			const Point baseAxisPointCapsuleLocal = getStart() + glm::clamp(directionAxisComponent, 0.f, 1.f) * getAxis();
			const Vector normalBaseAxisPointToPointDirectionCapsuleLocal = (directionCapsuleLocal - baseAxisPointCapsuleLocal).getDirection();

			const Point nearSurfacePointCapsuleLocal = baseAxisPointCapsuleLocal + ellipsiodSurfacePointOffset(normalBaseAxisPointToPointDirectionCapsuleLocal);
			const Point nearSurfacePoint = Math::transform(nearSurfacePointCapsuleLocal, transCapsuleLocalToVectorSpace);
			return nearSurfacePoint;
		}
		
		virtual AABB getBounds() const override {
			return AABB(getStart() - getRadius(), getEnd() + getRadius());
		}

		virtual Collider::Type getType() const { return Collider::Type::Capsule; }
	protected:
		Vector ellipsiodSurfacePointOffset(const Vector& direction) const {
			scalar t = 0.f;
			for(Dimension i = 0; i < dim; i++) {
				t += (direction[i] * direction[i]) / (m_Radius[i] * m_Radius[i]);
			}
			return (1.f / glm::sqrt(t)) * m_Radius;
		}
	protected:
		Point m_Start = Point(0.f);
		Point m_End = Point(0.f);
		Vector m_Radius = Vector();
	};

	template<Dimension dim>
	CapsuleCollider<dim> transform(const CapsuleCollider<dim>& collider, const Transform<dim>& transform) {
		return CapsuleCollider<dim>(Math::transform(collider.getStart(), transform.toMat()), Math::transform(collider.getEnd(), transform.toMat()), Math::transform(collider.getRadius(), transform.toMat()));
	}

}