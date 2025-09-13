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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using AABBD = AABB<dim>;
		using ColliderD = Collider<dim>;
		using TransformD = Transform<dim>;
	public:
		CapsuleCollider() = default;
		CapsuleCollider(PointD start, PointD end, scalar radius) : ColliderD(), m_Start(start), m_End(end), m_Radius(VectorD(radius)) { }
		CapsuleCollider(PointD start, PointD end, VectorD radius) : ColliderD(), m_Start(start), m_End(end), m_Radius(radius) { }
		CapsuleCollider(CapsuleCollider&) = default;
		CapsuleCollider(const CapsuleCollider&) = default;
		~CapsuleCollider() = default;

		PointD getStart() const {
			return m_Start;
		}
		PointD getEnd() const {
			return m_End;
		}
		VectorD getRadius() const { // HalfExtent
			return m_Radius;
		}
		VectorD getAxis() const {
			return m_End - m_Start;
		}
		PointD getCenter() const {
			return (getStart() + getEnd()) * .5f;
		}

		/**
		 * @brief calculates the nearest surface point on the Capsule to the given point
		 * @param transformToPointSpace the Capsules Transform in the points space, probably world space -> the Capsules World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			const matd transCapsuleLocalToPointSpace = transformToPointSpace.toMat();
			const matd transPointSpaceToCapsuleLocal = transformToPointSpace.toInvMat();

			const PointD pointCapsuleLocal = Math::transform(point, transPointSpaceToCapsuleLocal);

			// Axis from Start to End, check startLocal point vector -> axis relative length projection of point on axis (,0] (0,1) [1,)
			const scalar pointAxisComponent = getAxis().dot(pointCapsuleLocal - getStart()) / getAxis().lengthSquared();

			const PointD baseAxisPointCapsuleLocal = getStart() + glm::clamp(pointAxisComponent, 0.f, 1.f) * getAxis();
			const VectorD normalBaseAxisPointToPointDirectionCapsuleLocal = (pointCapsuleLocal - baseAxisPointCapsuleLocal).getDirection();

			const PointD nearSurfacePointCapsuleLocal = baseAxisPointCapsuleLocal + ellipsiodSurfacePointOffset(normalBaseAxisPointToPointDirectionCapsuleLocal);
			const PointD nearSurfacePoint = Math::transform(nearSurfacePointCapsuleLocal, transCapsuleLocalToPointSpace);
			return nearSurfacePoint;
		}

		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			const matd transCapsuleLocalToVectorSpace = transformToVectorSpace.toMat();
			const matd transVectorSpaceToCapsuleLocal = transformToVectorSpace.toInvMat();

			const VectorD directionCapsuleLocal = Math::transform(direction, transVectorSpaceToCapsuleLocal);

			// Axis from Start to End, check startLocal point vector -> axis relative length projection of point on axis (,0] (0,1) [1,)
			const scalar directionAxisComponent = getAxis().dot(directionCapsuleLocal) / getAxis().lengthSquared();

			const PointD baseAxisPointCapsuleLocal = getStart() + glm::clamp(directionAxisComponent, 0.f, 1.f) * getAxis();
			const VectorD normalBaseAxisPointToPointDirectionCapsuleLocal = (directionCapsuleLocal - baseAxisPointCapsuleLocal).getDirection();

			const PointD nearSurfacePointCapsuleLocal = baseAxisPointCapsuleLocal + ellipsiodSurfacePointOffset(normalBaseAxisPointToPointDirectionCapsuleLocal);
			const PointD nearSurfacePoint = Math::transform(nearSurfacePointCapsuleLocal, transCapsuleLocalToVectorSpace);
			return nearSurfacePoint;
		}
		
		virtual AABBD getBounds() const override {
			return AABBD(getStart() - getRadius(), getEnd() + getRadius());
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::Capsule; }
	protected:
		VectorD ellipsiodSurfacePointOffset(const VectorD& direction) const {
			scalar t = 0.f;
			for(Dimension i = 0; i < dim; i++) {
				t += (direction[i] * direction[i]) / (m_Radius[i] * m_Radius[i]);
			}
			return (1.f / glm::sqrt(t)) * m_Radius;
		}
	protected:
		PointD m_Start = PointD(0.f);
		PointD m_End = PointD(0.f);
		VectorD m_Radius = VectorD();
	};

	template<Dimension dim>
	CapsuleCollider<dim> transform(const CapsuleCollider<dim>& collider, const Transform<dim>& transform) {
		return CapsuleCollider<dim>(Math::transform(collider.getStart(), transform.toMat()), Math::transform(collider.getEnd(), transform.toMat()), Math::transform(collider.getRadius(), transform.toMat()));
	}

}