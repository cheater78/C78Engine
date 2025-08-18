#pragma once
#include "CollisionObject.h"

namespace C78E::Physics {

	template <Dimension dim>
	class CollisionSolver {
	public:
		using vecd = vec<dim>;
		virtual ~CollisionSolver() = default;

		virtual void solve(std::vector<Collision<dim>>& collisions, Timestep deltaTime) = 0;

	};

}