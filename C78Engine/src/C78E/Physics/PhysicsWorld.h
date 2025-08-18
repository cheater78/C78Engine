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
		using Rotation = Rotation<dim>;
		using Transform = Transform<dim>;
		using Collider = Collider<dim>;
		using CollisionObject = CollisionObject<dim>;
		using CollisionPoints = CollisionPoints<dim>;
		using Collision = Collision<dim>;
		using CollisionTest = CollisionTest<dim>;
		using CollisionSolver = CollisionSolver<dim>;
		using CollisionCallbackFunc = CollisionCallbackFunc<dim>;
	public:
		CollisionWorld() = default;
		CollisionWorld(CollisionWorld&) = delete;
		virtual ~CollisionWorld() = default;
		
		void addCollisionObject(CollisionObject* object) {
			m_Objects.emplace_back(object);
		}
		void removeCollisionObject(CollisionObject* object) {
			auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
			if(it == m_Objects.end()) return;
			m_Objects.erase(it); //TODO: performance concern
		}

		void addCollisionSolver(CollisionSolver* collisionSolver) {
			m_CollisionSolvers.emplace_back(collisionSolver);
		}
		void removeCollisionSolver(CollisionSolver* collisionSolver) {
			auto it = std::find(m_CollisionSolvers.begin(), m_CollisionSolvers.end(), collisionSolver);
			if(it == m_CollisionSolvers.end()) return;
			m_CollisionSolvers.erase(it); //TODO: performance concern
		}

		void resolveCollisions(Timestep deltaTime) {
			std::vector<Collision> collisions;
			std::vector<Collision> triggers;

			detectCollisions(collisions, triggers);

			solveCollisions(collisions, deltaTime);

			callCollisionCallbacks(collisions, deltaTime);
			callCollisionCallbacks(triggers, deltaTime);
		}
	private:
		void detectCollisions(std::vector<Collision>& collisions, std::vector<Collision>& triggers) {
			for(CollisionObject* a : m_Objects) {
				for(CollisionObject* b : m_Objects) {
					if(!a || !b) continue;
					if(a == b) break; // break inner Loop on Equality, unique upper Triangle of kartesian Product

					const Collider* colliderA = a->getCollider();
					const Collider* colliderB = b->getCollider();
					Transform& transformA = a->getTransform();
					Transform& transformB = b->getTransform();

					CollisionPoints points = CollisionTest::calculateCollision(colliderA, transformA, colliderB, transformB);

					if(points.hasCollision) {
						CollisionObject* coA = (!points.swapped) ? a : b;
						CollisionObject* coB = (!points.swapped) ? b : a;
						if(a->isTrigger() && b->isTrigger()) triggers.emplace_back(coA, coB, points);
						else collisions.emplace_back(coA, coB, points);
					}
				}
			}
		}
		void solveCollisions(std::vector<Collision>& collisions, Timestep deltaTime) {
			for(CollisionSolver* collisionSolver : m_CollisionSolvers) {
				collisionSolver->solve(collisions, deltaTime);
			}
		}
		static void callCollisionCallbacks(std::vector<Collision>& collisions, Timestep deltaTime) {
			for(Collision& collision : collisions) {
				collision.objA->callCollisionCallback(collision, deltaTime);
				collision.objB->callCollisionCallback(collision, deltaTime);
			}
		}
	protected:
		std::vector<CollisionObject*> m_Objects; // out of class storage! -> * or & or Ref<>
		std::vector<CollisionSolver*> m_CollisionSolvers; // out of class storage! -> * or & or Ref<>

		CollisionCallbackFunc m_OnAnyCollision = nullptr;
	};

	template<Dimension dim>
	class DynamicsWorld : public CollisionWorld<dim> {
	public:
		using vecd = vec<dim>;
		using Rotation = Rotation<dim>;
		using Transform = Transform<dim>;
		using CollisionObject = CollisionObject<dim>;
		using RigidBody = RigidBody<dim>;
		using CollisionWorld = CollisionWorld<dim>;
	public:
		DynamicsWorld() = default;
		DynamicsWorld(DynamicsWorld&) = delete;
		virtual ~DynamicsWorld() = default;

		void step(Timestep deltaTime) {
			const float dt = deltaTime.getSeconds();
			CollisionWorld::resolveCollisions(deltaTime);

			for(CollisionObject* obj : CollisionWorld::m_Objects) {
				RigidBody* dynamicBody = dynamic_cast<RigidBody*>(obj);
				if(!dynamicBody) continue; // Dynamics only!
				RigidBody& rigidBody = *dynamicBody;

				// Apply Acceleration
				rigidBody.applyGravity(m_Gravity); //apply bodyGravity or worldGravity

				// Apply Velocity
				vecd& bodyVelocity = rigidBody.getVelocity();
				Rotation& bodyAngularVelocity = rigidBody.getAngularVelocity();

				bodyVelocity += rigidBody.getForce() / rigidBody.getMass() * dt;
				bodyAngularVelocity += Rotation(rigidBody.getTorque().toMat() * rigidBody.getInvInertia() * dt);

				// Apply Position
				Transform& transform = rigidBody.getTransform();

				if(rigidBody.hasAngularVelocity()) {
					Rotation rotation = transform.getRotation();
					rotation += bodyAngularVelocity.toMat() * dt;
					transform.setRotation(rotation);
				}

				Point<dim> translation = transform.getTranslation();
				translation += Vector<dim>(rigidBody.getVelocity()) * dt;
				transform.setTranslation(translation);

				// Clear Forces
				rigidBody.getForce() = vecd(0.f);
				rigidBody.getTorque() = Rotation();
			}

		}

		void setGravity(vecd gravity) {
			m_Gravity = gravity;
		}
	private:
		vecd m_Gravity = vecd(0.f); // TODO more generic
	};

}