#pragma once
#include "HullCollider.h"
//#include <C78E/Asset/Mesh/Mesh.h>

namespace C78E::Physics {

	/**
	 * @brief HullCollider, for convex Meshes
	 * @tparam dim
	 */
	template<Dimension dim>
	struct MeshCollider : protected AABBCollider<dim> {
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using AABB = AABB<dim>;
		using Collider = Collider<dim>;
		using AABBCollider = AABBCollider<dim>;
		using HullCollider = HullCollider<dim>;
		using DynamicHullColliders = DynamicHullColliders<dim>;
		using Transform = Transform<dim>;
	public:
		MeshCollider() = default;
		MeshCollider(MeshCollider&) = default;
		MeshCollider(const MeshCollider&) = default;
		~MeshCollider() = default;

		AABB& calcBounds() {
			AABB::reset();
			for(const HullCollider& hull : m_HullColliders) {
				AABB::growToInclude(hull.getBounds());
			}
			return *this;
		}
		
		const DynamicHullColliders& getHullColliders() const {
			return m_HullColliders;
		}

		/**
		 * @brief calculates the nearest surface point on the Mesh to the given point
		 * @param transformToPointSpace the Meshes Transform in the points space, probably world space -> the Meshes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual Point nearSurfacePoint(Transform& transformToPointSpace, const Point& point) const override {
			//TODO:
			return Point();
		}

		virtual Point nearSurfacePoint(Transform& transformToVectorSpace, const Vector& direction) const override {
			//TODO:
			return Point();
		}

		virtual AABB getBounds() const override {
			return AABBCollider::getBounds();
		}

		virtual Collider::Type getType() const { return Collider::Type::Mesh; }
	public:
		//Ref<Mesh> m_Mesh = nullptr;
		DynamicHullColliders m_HullColliders;
	};

	template<Dimension dim>
	MeshCollider<dim> transform(const MeshCollider<dim>& collider, const Transform<dim>& transform) {
		MeshCollider<dim> transformedCollider = collider;
		for(const HullCollider<dim>& hull : transformedCollider.getHullColliders()) {
			hull = Math::transform(hull, transform);
		}
		return transformedCollider;
	}

}