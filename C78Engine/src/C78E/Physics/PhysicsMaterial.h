#pragma once
#include <C78E/Math/Math.h>

namespace C78E::Physics {

	struct PhysicsMaterial {
	public:
		PhysicsMaterial() = default;
		PhysicsMaterial(const PhysicsMaterial& other) {
			dynamicFriction = other.dynamicFriction;
			staticFriction = other.staticFriction;
			restitution = other.restitution;
		}
		~PhysicsMaterial() = default;

		scalar dynamicFriction = 0.0f;
		scalar staticFriction = 0.0f;
		scalar restitution = 1.0f;
	};

}