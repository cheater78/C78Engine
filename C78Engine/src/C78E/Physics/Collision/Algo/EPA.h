#pragma once
#include <C78E/Physics/Collision/CollisionPoints.h>
#include <C78E/Physics/Collision/Collider/Collider.h>
#include "Simplex.h"

namespace C78E::Physics {

#ifndef EPA_MAX_ITER
#define EPA_MAX_ITER 32
#endif // !EPA_MAX_ITER

	template<Dimension dim>
	struct EPA {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Simplex = Simplex<dim>;
		using Transform = Transform<dim>;
		using Collider = Collider<dim>;
		using CollisionPoints = CollisionPoints<dim>;

		static CollisionPoints calculate(
			const Simplex& initialSimplex,
			const Collider& colliderA, Transform& transformA,
			const Collider& colliderB, Transform& transformB) {
			CollisionPoints result;

			Simplex simplex = initialSimplex; // Copy to local, may be modified
			Vector direction = estimateInitialDirection(simplex); // Use last GJK dir or cross edge

			for(size_t i = 0; i < EPA_MAX_ITER; ++i) {
				Point support = Point(Collider::calculateColliderDistanceVec(colliderA, transformA, colliderB, transformB, direction).getDirection());

				// Compute distance from support to origin in the direction
				scalar supportDist = support.getOriginVector().dot(direction);

				// Closest distance so far (used as penetration depth)
				scalar closestDist = direction.lengthSquared(); // direction should already point toward origin

				// Termination condition: new support doesn't improve
				if(isNearZero(supportDist - sqrt(closestDist))) {
					result.hasCollision = true;
					result.depth = sqrt(closestDist);
					result.normal = direction.normalize();

					// Calculate closest surface points A and B
					Point a = colliderA.nearSurfacePoint(transformA, +result.normal);
					Point b = colliderB.nearSurfacePoint(transformB, -result.normal);

					result.a = a;
					result.b = b;
					return result;
				}

				// Add new point to the polytope
				simplex.push_front(support);

				// Recalculate direction toward closest face to origin
				if(!findClosestFace(simplex, direction)) {
					break;
				}
			}

			// Failed to converge
			result.hasCollision = false;
			result.depth = -scalar_limit::max();
			return result;
		}

	private:
		static Vector estimateInitialDirection(const Simplex& simplex) {
			// Fall back to some meaningful direction from simplex if possible
			if(simplex.size() >= 2) {
				Vector a = simplex[0].getOriginVector();
				Vector b = simplex[1].getOriginVector();
				return b - a;
			}
			return Vector(1.f); // Fallback direction if simplex is degenerate
		}

		static bool findClosestFace(Simplex& simplex, Vector& outDirection) {
			// Simplified: use projection of origin onto simplex edges or faces
			// For 2D, this is just the closest edge normal pointing toward origin
			// For N-D, this needs to be the face normal that most directly faces the origin

			Vector closestNormal = vecd(0);
			scalar minDistance = scalar_limit::max();

			for(size_t i = 0; i < simplex.size(); ++i) {
				const Point& p0 = simplex[i];
				const Point& p1 = simplex[(i + 1) % simplex.size()];
				Vector edge = p1 - p0;

				// Find perpendicular to edge (2D), or cross products in higher dims
				Vector normal = edge.constructNormal();
				if(Math::isNearZero(normal.lengthSquared())) continue;
				normal = normal.normalize();
				scalar dist = normal.dot(p0.getOriginVector()); // distance from origin

				if(dist < minDistance && dist > 0) {
					minDistance = dist;
					closestNormal = normal;
				}
			}

			if(!isNearZero(closestNormal.lengthSquared())) {
				outDirection = closestNormal;
				return true;
			}
			return false;
		}
	};

}