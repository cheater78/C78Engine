#pragma once
#include "CollisionSolver.h"

namespace C78E::Physics {

	template <Dimension dim>
	class PositionSolver : public CollisionSolver<dim> {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Transform = Transform<dim>;
		using CollisionObject = CollisionObject<dim>;
		using RigidBody = RigidBody<dim>;
		virtual ~PositionSolver() = default;

		virtual void solve(std::vector<Collision<dim>>& collisions, Timestep deltaTime) override {
			for(Collision<dim>& collision : collisions) {
				CollisionObject* aBody = collision.objA;
				CollisionObject* bBody = collision.objB;

				Transform& aTransform = aBody->getTransform();
				Transform& bTransform = bBody->getTransform();

				const scalar aIsStatic = static_cast<scalar>(aBody->isStatic());
				const scalar bIsStatic = static_cast<scalar>(bBody->isStatic());

				vecd resolution = collision.points.normal * collision.points.depth / glm::max(1.f, aIsStatic + bIsStatic);

				aTransform.setTranslation(aTransform.getTranslation() + Vector(resolution * (1.f - aIsStatic)));
				bTransform.setTranslation(bTransform.getTranslation() - Vector(resolution * (1.f - bIsStatic)));
			}
		}

	};

}