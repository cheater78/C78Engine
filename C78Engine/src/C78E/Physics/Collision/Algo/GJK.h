#pragma once
#include <C78E/Physics/Collision/CollisionPoints.h>
#include <C78E/Physics/Collision/Collider/Collider.h>
#include "Simplex.h"
#include "EPA.h"

namespace C78E::Physics {

#ifndef GJK_MAX_ITER
#define GJK_MAX_ITER 32
#endif

	template<Dimension dim>
	struct GJK {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		template<PointsCount size>
		using Points = Points<dim, size>;
		template<VectorsCount size>
		using Vectors = Vectors<dim, size>;
		using AABB = AABB<dim>;
		using Transform = Transform<dim>;
		using Simplex = Simplex<dim>;
		using Collider = Collider<dim>;
	public:
		static std::pair<bool, Simplex> calculate(const Collider& colliderA, Transform& transformA, const  Collider& colliderB, Transform& transformB) {
			const AABB boxAworld = Math::transform(colliderA.getBounds(), transformA.toMat());
			const AABB boxBworld = Math::transform(colliderB.getBounds(), transformB.toMat());

			const Vector colliderDistance = (boxAworld.getCenter() - boxBworld.getCenter()).refit<dim>();

			Point support = Point(Collider::calculateColliderDistanceVec(colliderA, transformA, colliderB, transformB, colliderDistance).getDirection());

			Simplex points;
			points.push_front(support);

			Vector direction = -support.getOriginVector();

			size_t iterations = 0;
			while(iterations++ < GJK_MAX_ITER) {
				support = Point(Collider::calculateColliderDistanceVec(colliderA, transformA, colliderB, transformB, direction).getDirection());

				if(support.getOriginVector().dot(direction) <= 0) {
					break;
				}

				points.push_front(support);

				if(nextSimplex(points, direction)) {
					return std::make_pair(true, points);
				}
			}

			return std::make_pair(false, points);
		}
	private:
		static bool nextSimplex(Simplex& points, Vector& direction) {
			const size_t size = points.size();
			if(size < 2) {
				return false;
			}

			Vectors<dim + 1> relative;
			for(size_t i = 0; i < size; ++i) {
				if(i == 0) {
					relative[i] = Point(0.f) - points[0];
				} else {
					relative[i] = points[i] - points[0];
				}
			}

			// Iterate over all faces of the simplex (excluding one point each time)
			for(size_t i = 1; i < size; ++i) {
				Vectors<dim - 1> subspace;
				size_t idx = 0;
				for(size_t j = 0; j < size; ++j) {
					if(j != i) {
						subspace[idx++] = relative[j];
					}
				}

				Vector normal = Math::cross(subspace);
				if(normal.alignsWith(relative[0])) {
					// Point lies beyond this face
					// Modify simplex to contain this face + origin-facing direction
					points.remove(i);
					direction = normal;
					return false;
				}
			}

			return true; // Origin is inside the current simplex
		}


	};

}