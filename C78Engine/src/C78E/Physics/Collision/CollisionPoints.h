#pragma once
#include <C78E/Math/Math.h>

namespace C78E::Physics {

	template<Dimension dim>
	struct CollisionPoints {
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;

		Point a = Point(0.f);									// closest Point of Collider A to B
		Point b = Point(0.f);									// closest Point of Collider B to A
		Vector normal = Vector();							// normalized direction from A to B
		scalar depth = -scalar_limit::max();	// longest Distance in Overlap, original length of the (non normalized) normal, negative if no Collision
		bool hasCollision = false;							// whether a collision has occured -> depth > 0
		bool swapped = false;								// keeps track if a and b were swap through double dispatch or TestCollision Call

		CollisionPoints<dim>& swap() { // call if A and B were swapped, returns it self for inline
			swapped = !swapped;
			return *this;
		}
	};

}