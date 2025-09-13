#pragma once
#include "CollisionSolver.h"

namespace C78E::Physics {

	template <Dimension dim>
	class PositionSolver : public CollisionSolver<dim> {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using TransformD = Transform<dim>;
		using CollisionObjectD = CollisionObject<dim>;
		using RigidBodyD = RigidBody<dim>;
		virtual ~PositionSolver() = default;

		virtual void solve(std::vector<Collision<dim>>& collisions, Timestep deltaTime) override {
			for(Collision<dim>& collision : collisions) {
				CollisionObjectD* aBody = collision.objA;
				CollisionObjectD* bBody = collision.objB;

				TransformD& aTransform = aBody->getTransform();
				TransformD& bTransform = bBody->getTransform();

				const scalar aIsStatic = static_cast<scalar>(aBody->isStatic());
				const scalar bIsStatic = static_cast<scalar>(bBody->isStatic());

				vecd resolution = collision.points.normal * collision.points.depth / glm::max(1.f, aIsStatic + bIsStatic);

				aTransform.setTranslation(aTransform.getTranslation() + VectorD(resolution * (1.f - aIsStatic)));
				bTransform.setTranslation(bTransform.getTranslation() - VectorD(resolution * (1.f - bIsStatic)));
			}
		}

	};

}