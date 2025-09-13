#pragma once
#include "Collision/CollisionTest.h"
#include "CollisionSolver.h"
#include "PositionSolver.h"
#include "ImpulseSolver.h"

namespace C78E::Physics {

	template<Dimension dim>
	class CollisionWorld {
	public:
		using vecd = vec<dim>;
		using RotationD = Rotation<dim>;
		using TransformD = Transform<dim>;
		using ColliderD = Collider<dim>;
		using CollisionObjectD = CollisionObject<dim>;
		using CollisionPointsD = CollisionPoints<dim>;
		using CollisionD = Collision<dim>;
		using CollisionTestD = CollisionTest<dim>;
		using CollisionSolverD = CollisionSolver<dim>;
		using CollisionCallbackFuncD = CollisionCallbackFunc<dim>;
	public:
		CollisionWorld() = default;
		CollisionWorld(CollisionWorld&) = delete;
		virtual ~CollisionWorld() = default;
		
		void addCollisionObject(CollisionObjectD* object) {
			m_Objects.emplace_back(object);
		}
		void removeCollisionObject(CollisionObjectD* object) {
			auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
			if(it == m_Objects.end()) return;
			m_Objects.erase(it); //TODO: performance concern
		}

		void addCollisionSolver(CollisionSolverD* collisionSolver) {
			m_CollisionSolvers.emplace_back(collisionSolver);
		}
		void removeCollisionSolver(CollisionSolverD* collisionSolver) {
			auto it = std::find(m_CollisionSolvers.begin(), m_CollisionSolvers.end(), collisionSolver);
			if(it == m_CollisionSolvers.end()) return;
			m_CollisionSolvers.erase(it); //TODO: performance concern
		}

		void resolveCollisions(Timestep deltaTime) {
			std::vector<CollisionD> collisions;
			std::vector<CollisionD> triggers;

			detectCollisions(collisions, triggers);

			solveCollisions(collisions, deltaTime);

			callCollisionCallbacks(collisions, deltaTime);
			callCollisionCallbacks(triggers, deltaTime);
		}
	private:
		void detectCollisions(std::vector<CollisionD>& collisions, std::vector<CollisionD>& triggers) {
			for(CollisionObjectD* a : m_Objects) {
				for(CollisionObjectD* b : m_Objects) {
					if(!a || !b) continue;
					if(a == b) break; // break inner Loop on Equality, unique upper Triangle of kartesian Product

					const ColliderD* colliderA = a->getCollider();
					const ColliderD* colliderB = b->getCollider();
					TransformD& transformA = a->getTransform();
					TransformD& transformB = b->getTransform();

					CollisionPointsD points = CollisionTestD::calculateCollision(colliderA, transformA, colliderB, transformB);

					if(points.hasCollision) {
						CollisionObjectD* coA = (!points.swapped) ? a : b;
						CollisionObjectD* coB = (!points.swapped) ? b : a;
						if(a->isTrigger() && b->isTrigger()) triggers.emplace_back(coA, coB, points);
						else collisions.emplace_back(coA, coB, points);
					}
				}
			}
		}
		void solveCollisions(std::vector<CollisionD>& collisions, Timestep deltaTime) {
			for(CollisionSolverD* collisionSolver : m_CollisionSolvers) {
				collisionSolver->solve(collisions, deltaTime);
			}
		}
		static void callCollisionCallbacks(std::vector<CollisionD>& collisions, Timestep deltaTime) {
			for(CollisionD& collision : collisions) {
				collision.objA->callCollisionCallback(collision, deltaTime);
				collision.objB->callCollisionCallback(collision, deltaTime);
			}
		}
	protected:
		std::vector<CollisionObjectD*> m_Objects; // out of class storage! -> * or & or Ref<>
		std::vector<CollisionSolverD*> m_CollisionSolvers; // out of class storage! -> * or & or Ref<>

		CollisionCallbackFuncD m_OnAnyCollision = nullptr;
	};

	template<Dimension dim>
	class DynamicsWorld : public CollisionWorld<dim> {
	public:
		using vecd = vec<dim>;
		using RotationD = Rotation<dim>;
		using TransformD = Transform<dim>;
		using CollisionObjectD = CollisionObject<dim>;
		using RigidBodyD = RigidBody<dim>;
		using CollisionWorldD = CollisionWorld<dim>;
	public:
		DynamicsWorld() = default;
		DynamicsWorld(DynamicsWorld&) = delete;
		virtual ~DynamicsWorld() = default;

		void step(Timestep deltaTime) {
			const float dt = deltaTime.getSeconds();
			CollisionWorldD::resolveCollisions(deltaTime);

			for(CollisionObjectD* obj : CollisionWorldD::m_Objects) {
				RigidBodyD* dynamicBody = dynamic_cast<RigidBodyD*>(obj);
				if(!dynamicBody) continue; // Dynamics only!
				RigidBodyD& rigidBody = *dynamicBody;

				// Apply Acceleration
				rigidBody.applyGravity(m_Gravity); //apply bodyGravity or worldGravity

				// Apply Velocity
				vecd& bodyVelocity = rigidBody.getVelocity();
				RotationD& bodyAngularVelocity = rigidBody.getAngularVelocity();

				bodyVelocity += rigidBody.getForce() / rigidBody.getMass() * dt;
				bodyAngularVelocity += RotationD(rigidBody.getTorque().toMat() * rigidBody.getInvInertia() * dt);

				// Apply Position
				TransformD& transform = rigidBody.getTransform();

				if(rigidBody.hasAngularVelocity()) {
					RotationD rotation = transform.getRotation();
					rotation += bodyAngularVelocity.toMat() * dt;
					transform.setRotation(rotation);
				}

				Point<dim> translation = transform.getTranslation();
				translation += Vector<dim>(rigidBody.getVelocity()) * dt;
				transform.setTranslation(translation);

				// Clear Forces
				rigidBody.getForce() = vecd(0.f);
				rigidBody.getTorque() = RotationD();
			}

		}

		void setGravity(vecd gravity) {
			m_Gravity = gravity;
		}
	private:
		vecd m_Gravity = vecd(0.f); // TODO more generic
	};

}