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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		template<PointsCount size>
		using PointsD = Points<dim, size>;
		template<VectorsCount size>
		using VectorsD = Vectors<dim, size>;
		using AABBD = AABB<dim>;
		using TransformD = Transform<dim>;
		using SimplexD = Simplex<dim>;
		using ColliderD = Collider<dim>;
	public:
		static std::pair<bool, SimplexD> calculate(const ColliderD& colliderA, TransformD& transformA, const  ColliderD& colliderB, TransformD& transformB) {
			const AABBD boxAworld = Math::transform(colliderA.getBounds(), transformA.toMat());
			const AABBD boxBworld = Math::transform(colliderB.getBounds(), transformB.toMat());

			const VectorD colliderDistance = (boxAworld.getCenter() - boxBworld.getCenter());

			PointD support = PointD(ColliderD::calculateColliderDistanceVec(colliderA, transformA, colliderB, transformB, colliderDistance).getDirection());

			SimplexD points;
			points.push_front(support);

			VectorD direction = -support.getOriginVector();

			size_t iterations = 0;
			while(iterations++ < GJK_MAX_ITER) {
				support = PointD(ColliderD::calculateColliderDistanceVec(colliderA, transformA, colliderB, transformB, direction).getDirection());

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
		static bool nextSimplex(SimplexD& points, VectorD& direction) {
			const size_t size = points.size();
			if(size < 2) {
				return false;
			}

			VectorsD<dim + 1> relative;
			for(size_t i = 0; i < size; ++i) {
				if(i == 0) {
					relative[i] = PointD(0.f) - points[0];
				} else {
					relative[i] = points[i] - points[0];
				}
			}

			// Iterate over all faces of the simplex (excluding one point each time)
			for(size_t i = 1; i < size; ++i) {
				VectorsD<dim - 1> subspace;
				size_t idx = 0;
				for(size_t j = 0; j < size; ++j) {
					if(j != i) {
						subspace[idx++] = relative[j];
					}
				}

				VectorD normal = Math::cross(subspace);
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