#pragma once
#include <C78E/Math/Math.h>
#include "CollisionPoints.h"
#include "Collider/Collider.h"

#include "Algo/SAT.h"
#include "Algo/GJK.h"
#include "Algo/EPA.h"


namespace C78E::Physics {

	template<Dimension dim>
	struct CollisionTest {
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Plane = Plane<dim>;
		using Sphere = Sphere<dim>;
		using AABBD = AABB<dim>;
		using Transform = Transform<dim>;
		using ColliderD = Collider<dim>;
		using PlaneColliderD = PlaneCollider<dim>;
		using SphereColliderD = SphereCollider<dim>;
		using AABBColliderD = AABBCollider<dim>;
		using CapsuleColliderD = CapsuleCollider<dim>;
		using CylinderColliderD = CylinderCollider<dim>;
		using HullColliderD = HullCollider<dim>;
		using MeshColliderD = MeshCollider<dim>;
		using CollisionPointsD = CollisionPoints<dim>;
	public:
		static CollisionPointsD calculateCollision(const ColliderD* colliderA, Transform& transformA, const ColliderD* colliderB, Transform& transformB) {
			const typename ColliderD::Type typeA = colliderA->getType();
			const typename ColliderD::Type typeB = colliderB->getType();

			if(typeA <= typeB) { // no swap
				const CalculateCollisionFunction& ccf = calculateCollisionFunctions[(size_t)typeA][(size_t)typeB];
				if(ccf) return ccf(colliderA, transformA, colliderB, transformB);
			} else {
				const CalculateCollisionFunction& ccf = calculateCollisionFunctions[(size_t)typeB][(size_t)typeA];
				if(ccf) return ccf(colliderB, transformB, colliderA, transformA).swap();
			}
			return CollisionPointsD();
		}
	private:
		using CalculateCollisionFunction = std::function<CollisionPointsD(const ColliderD*, Transform&, const ColliderD*, Transform&)>;
		
		template<typename ColliderTypeA, typename ColliderTypeB>
		static std::pair<const ColliderTypeA*, const ColliderTypeB*> castColliderType(const ColliderD* colliderA, const ColliderD* colliderB) {
			const ColliderTypeA* ptrA = dynamic_cast<const ColliderTypeA*>(colliderA);
			const ColliderTypeB* ptrB = dynamic_cast<const ColliderTypeB*>(colliderB);
			C78E_CORE_ASSERT(ptrA && ptrB, "CollisionTest::castColliderType: provided Colliders did not match the required Types! Types were {} and {}", (size_t)colliderA->getType(), (size_t)colliderB->getType());
			return std::make_pair(ptrA, ptrB);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Plane                                                                                                                  //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		static CollisionPointsD ccfPlaneThing(const ColliderD* baseColliderA, Transform& transformA, const ColliderD* baseColliderB, Transform& transformB) {
			auto [colliderA, colliderB] = castColliderType<PlaneColliderD, ColliderD>(baseColliderA, baseColliderB);

			const Plane planeWorld = Math::transform(*colliderA, transformA.toMat());

			const Point b = colliderB->nearSurfacePoint(transformB, -planeWorld.getNormal());
			const Point a = colliderA->nearSurfacePoint(transformA, b);

			const Vector collisionVectorWorld = b - a;

			CollisionPointsD points;
			if(!collisionVectorWorld.alignsWith(planeWorld.getNormal())) {
				points.hasCollision = true;
				points.a = a;
				points.b = b;
				points.normal = collisionVectorWorld.normalize();
				points.depth = collisionVectorWorld.length();
			}
			return points;
		}
		

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Sphere                                                                                                                 //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		static CollisionPointsD ccfSphereThing(const ColliderD* baseColliderA, Transform& transformA, const ColliderD* baseColliderB, Transform& transformB) {
			auto [colliderA, colliderB] = castColliderType<SphereColliderD, ColliderD>(baseColliderA, baseColliderB);

			const Sphere sphereWorld = Math::transform(colliderA->getSphere(), transformA.toMat());
			const AABBD colliderBBoundsWorld = Math::transform(colliderB->getBounds(), transformB.getTranslation(), transformB.getScale());

			const Point b = colliderB->nearSurfacePoint(transformB, sphereWorld.getCenter());
			const Point a = colliderA->nearSurfacePoint(transformA, b);

			const Vector collisionVectorWorld = b - a;
			const Vector centerToCenterVectorWorld = colliderBBoundsWorld.getCenter() - sphereWorld.getCenter();

			CollisionPointsD points;
			if(!centerToCenterVectorWorld.alignsWith(collisionVectorWorld)) {
				points.hasCollision = true;
				points.a = a;
				points.b = b;
				points.normal = collisionVectorWorld.normalize();
				points.depth = collisionVectorWorld.length();
			}
			return points;

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    AABB                                                                                                                   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Capsule                                                                                                                //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Cylinder                                                                                                               //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Hull                                                                                                                   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		static CollisionPointsD ccfGJK(const ColliderD* baseColliderA, Transform& transformA, const ColliderD* baseColliderB, Transform& transformB) {
			const AABBD boxAworld = Math::transform(baseColliderA->getBounds(), transformA.getTranslation(), transformA.getScale());
			const AABBD boxBworld = Math::transform(baseColliderB->getBounds(), transformB.getTranslation(), transformB.getScale());

			if(Math::intersection(boxAworld, boxBworld).hasNone()) return CollisionPointsD(); //TODO: AABB pre filter for all

			auto [collision, simplex] = GJK<dim>::calculate(*baseColliderA, transformA, *baseColliderB, transformB);

			if(collision) {
				return EPA<dim>::calculate(simplex, *baseColliderA, transformA, *baseColliderB, transformB);
			}

			return CollisionPointsD();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    Mesh                                                                                                                   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

		static CollisionPointsD getMaxPen(std::vector<CollisionPointsD>& pointss) {
			if(pointss.size() == 0) return CollisionPointsD();

			size_t maxNormalIndex = 0;
			scalar  maxNormalDist = scalar_limit::min();
			for(size_t i = 0; i < pointss.size(); i++) {
				if(pointss[i].depth > maxNormalDist) {
					maxNormalDist = pointss[i].depth;
					maxNormalIndex = i;
				}
			}
			return pointss[maxNormalIndex];
		}

		static CollisionPointsD ccfMeshMesh(const ColliderD* baseColliderA, Transform& transformA, const ColliderD* baseColliderB, Transform& transformB) {
			auto [colliderA, colliderB] = castColliderType<MeshColliderD, MeshColliderD>(baseColliderA, baseColliderB);

			std::vector<CollisionPointsD> pointss;
			for(const HullColliderD& aPart : colliderA->getHullColliders()) {
				for(const HullColliderD& bPart : colliderB->getHullColliders()) {

					{
						AABBD aBox = transform(aPart.getBounds(), transformA.getTranslation(), transformA.getScale());
						AABBD bBox = transform(bPart.getBounds(), transformB.getTranslation(), transformB.getScale());

						if(Math::intersection(aBox, bBox).hasNone()) continue;
					}

					auto [collision, simplex] = GJK<dim>::calculate(aPart, transformA, bPart, transformB);
					if(collision) {
						pointss.push_back(EPA<dim>::calculate(simplex, aPart, transformA, bPart, transformB));
					}
				}
			}

			return getMaxPen(pointss);
		}

		static CollisionPointsD ccfThingMesh(const ColliderD* baseColliderA, Transform& transformA, const ColliderD* baseColliderB, Transform& transformB) {
			auto [colliderA, colliderB] = castColliderType<ColliderD, MeshColliderD>(baseColliderA, baseColliderB);
			C78E_CORE_ASSERT(colliderA->getType() != ColliderD::Type::Plane && colliderA->getType() != ColliderD::Type::Mesh, "CollisionTest::ccfThingMesh: colliderA can't be Plane or Mesh!");
			AABBD aBox = Math::transform(colliderA->getBounds(), transformA.getTranslation(), transformA.getScale());

			std::vector<CollisionPointsD> pointss;
			for(const HullColliderD& part : colliderB->getHullColliders()) {
				{
					AABBD bBox = Math::transform(part.getBounds(), transformB.getTranslation(), transformB.getScale());

					if(Math::intersection(aBox, bBox).hasNone()) continue;
				}

				auto [collision, simplex] = GJK<dim>::calculate(*baseColliderA, transformA, part, transformB);
				if(collision) {
					pointss.push_back(EPA<dim>::calculate(simplex, *colliderA, transformA, part, transformB));
				}
			}

			return getMaxPen(pointss);
		}


	private:
	#define NoSemantic nullptr
	#define NoFunction nullptr
	#define StillTODO nullptr

		static inline const CalculateCollisionFunction calculateCollisionFunctions[(size_t)ColliderD::Type::TYPE_SIZE][(size_t)ColliderD::Type::TYPE_SIZE] {
			/* Functions		*/	/* NoCollider	*/	/* PlaneCollider	*/	/* SphereCollider	*/	/* AABBCollider		*/	/* CapsuleCollider	*/	/* CylinderCollider */	/* HullCollider		*/ /* MeshCollider	*/	/* Functions		*/
			/* NoCollider		*/	{ NoSemantic,			NoSemantic,				NoSemantic,				NoSemantic,				NoSemantic,				NoSemantic,				NoSemantic,			NoSemantic		},	/* NoCollider		*/
			/* PlaneCollider	*/	{ NoSemantic,			NoSemantic,				ccfPlaneThing,			ccfPlaneThing,			ccfPlaneThing,			ccfPlaneThing,			ccfPlaneThing,		ccfPlaneThing	},	/* PlaneCollider	*/
			/* SphereCollider	*/	{ NoSemantic,			NoFunction,				ccfSphereThing,			ccfSphereThing,			ccfSphereThing,			ccfSphereThing,			ccfSphereThing,		ccfThingMesh	},	/* SphereCollider	*/
			/* AABBCollider		*/	{ NoSemantic,			NoFunction,				NoFunction,				StillTODO,				StillTODO,				StillTODO,				ccfGJK,				ccfThingMesh	},	/* AABBCollider		*/
			/* CapsuleCollider	*/	{ NoSemantic,			NoFunction,				NoFunction,				NoFunction,				StillTODO,				StillTODO,				ccfGJK,				ccfThingMesh	},	/* CapsuleCollider	*/
			/* CylinderCollider */	{ NoSemantic,			NoFunction,				NoFunction,				NoFunction,				NoFunction,				StillTODO,				ccfGJK,				ccfThingMesh	},	/* CylinderCollider */
			/* HullCollider		*/	{ NoSemantic,			NoFunction,				NoFunction,				NoFunction,				NoFunction,				NoFunction,				ccfGJK,				ccfThingMesh	},	/* HullCollider		*/
			/* MeshCollider		*/	{ NoSemantic,			NoFunction,				NoFunction,				NoFunction,				NoFunction,				NoFunction,				NoFunction,			ccfMeshMesh		},	/* MeshCollider		*/
			/* Functions		*/	/* NoCollider	*/	/* PlaneCollider	*/	/* SphereCollider	*/	/* AABBCollider		*/	/* CapsuleCollider	*/	/* CylinderCollider */	/* HullCollider		*/ /* MeshCollider	*/	/* Functions		*/
		};

	#undef NoSemantic
	#undef NoFunction
	};

}