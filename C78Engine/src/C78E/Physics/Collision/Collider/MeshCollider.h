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
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using AABBD = AABB<dim>;
		using ColliderD = Collider<dim>;
		using AABBColliderD = AABBCollider<dim>;
		using HullColliderD = HullCollider<dim>;
		using DynamicHullCollidersD = DynamicHullColliders<dim>;
		using TransformD = Transform<dim>;
	public:
		MeshCollider() = default;
		MeshCollider(MeshCollider&) = default;
		MeshCollider(const MeshCollider&) = default;
		~MeshCollider() = default;

		AABBD& calcBounds() {
			AABBD::reset();
			for(const HullColliderD& hull : m_HullColliders) {
				AABBD::growToInclude(hull.getBounds());
			}
			return *this;
		}
		
		const DynamicHullCollidersD& getHullColliders() const {
			return m_HullColliders;
		}

		/**
		 * @brief calculates the nearest surface point on the Mesh to the given point
		 * @param transformToPointSpace the Meshes Transform in the points space, probably world space -> the Meshes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			//TODO:
			return PointD();
		}

		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			//TODO:
			return PointD();
		}

		virtual AABBD getBounds() const override {
			return AABBColliderD::getBounds();
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::Mesh; }
	public:
		//Ref<Mesh> m_Mesh = nullptr;
		DynamicHullCollidersD m_HullColliders;
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