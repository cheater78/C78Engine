#pragma once
#include "CollisionSolver.h"

namespace C78E::Physics {

	template <Dimension dim>
	class ImpulseSolver : public CollisionSolver<dim> {
	public:
		using vecd = vec<dim>;
		virtual ~ImpulseSolver() = default;

		virtual void solve(std::vector<Collision<dim>>& collisions, Timestep deltaTime) override {
			for(Collision<dim>& collision : collisions) {
				// Replaces non dynamic objects with default values.

				RigidBody<dim>* aBody = dynamic_cast<RigidBody<dim>*>(collision.objA);
				RigidBody<dim>* bBody = dynamic_cast<RigidBody<dim>*>(collision.objB);

				vecd aVel = aBody ? aBody->getVelocity() : vecd(0.0f);
				vecd bVel = bBody ? bBody->getVelocity() : vecd(0.0f);
				vecd rVel = bVel - aVel;
				scalar nSpd = glm::dot(rVel, collision.points.normal.getDirection());

				scalar aInvMass = aBody ? 1.f / aBody->getMass() : 1.0f;
				scalar bInvMass = bBody ? 1.f / bBody->getMass() : 1.0f;

				// Impulse

				// This is important for convergence
				// a negative impulse would drive the objects closer together
				if(nSpd <= 0.f)
					continue;

				scalar e = (aBody ? aBody->getMaterial().restitution : 1.0f)
					* (bBody ? bBody->getMaterial().restitution : 1.0f);

				scalar j = -(1.0f + e) * nSpd / (aInvMass + bInvMass);

				vecd impluse = j * collision.points.normal.getDirection();

				if(aBody && aBody->isSimulated()) {
					aVel -= impluse * aInvMass;
				}

				if(bBody && bBody->isSimulated()) {
					bVel += impluse * bInvMass;
				}

				// Friction

				rVel = bVel - aVel;
				nSpd = glm::dot(rVel, collision.points.normal.getDirection());

				vecd tangent = rVel - nSpd * collision.points.normal.getDirection();

				if(glm::length(tangent) > 0.0001f) { // safe normalize
					tangent = glm::normalize(tangent);
				}

				scalar fVel = glm::dot(rVel, tangent);

				scalar aSF = aBody ? aBody->getMaterial().staticFriction : 0.0f;
				scalar bSF = bBody ? bBody->getMaterial().staticFriction : 0.0f;
				scalar aDF = aBody ? aBody->getMaterial().dynamicFriction : 0.0f;
				scalar bDF = bBody ? bBody->getMaterial().dynamicFriction : 0.0f;
				scalar mu = (scalar)glm::vec2(aSF, bSF).length();

				scalar f = -fVel / (aInvMass + bInvMass);

				vecd friction;
				if(abs(f) < j * mu) {
					friction = f * tangent;
				} else {
					mu = glm::length(glm::vec2(aDF, bDF));
					friction = -j * tangent * mu;
				}

				if(aBody ? aBody->isSimulated() : false) {
					aBody->getVelocity() = aVel - friction * aInvMass;
				}

				if(bBody ? bBody->isSimulated() : false) {
					bBody->getVelocity() = bVel + friction * bInvMass;
				}
			}
		}

	};

}