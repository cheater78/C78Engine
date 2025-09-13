#pragma once
#include <C78E/Math/Math.h>

namespace C78E::Physics {
	
	/**
	 * @brief abstract Collider, implementations describe the shapes on which Collisions can be tested
	 * @tparam dim 2D or 2D
	 */
	template<Dimension dim>
	struct Collider {
	public:
		using vecd = vec<dim>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		using AABBD = AABB<dim>;
		using TransformD = Transform<dim>;

		enum class Type {
			None = 0, Plane, Sphere, AABB, Capsule, Cylinder, Hull, Mesh, TYPE_SIZE
		};
	public:
		static VectorD calculateColliderDistanceVec(const Collider<dim>& colliderA, TransformD& transformA, const Collider<dim>& colliderB, TransformD& transformB, const VectorD& direction) {
			return colliderB.nearSurfacePoint(transformB, -direction) - colliderA.nearSurfacePoint(transformA, +direction);
		}

	public:
		Collider() = default;
		Collider(Collider&) = default;
		Collider(const Collider&) = default;
		virtual ~Collider() = default;

		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const = 0;
		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const = 0;
		virtual AABBD getBounds() const = 0;
		virtual Type getType() const { return Type::None; }

		template<typename ColliderType>
		ColliderType* asPtr() {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of<Collider<dim>, ColliderType>::value), "Collider::as: ColliderType incompatible(does not have Base Collider<dim>)!");
			ColliderType* collider = dynamic_cast<ColliderType*>(this);
			C78E_CORE_ASSERT(collider, "Collider::asPtr: ColliderType* incompatible!");
			return collider;
		}

		template<typename ColliderType>
		const ColliderType* asPtr() const {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of<Collider<dim>, ColliderType>::value), "Collider::as: ColliderType incompatible(does not have Base Collider<dim>)!");
			const ColliderType* collider = dynamic_cast<const ColliderType*>(this);
			C78E_CORE_ASSERT(collider, "Collider::asPtr: ColliderType* incompatible!");
			return collider;
		}

		template<typename ColliderType>
		ColliderType& as() {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of<Collider<dim>, ColliderType>::value), "Collider::as: ColliderType incompatible(does not have Base Collider<dim>)!");
			ColliderType* collider = dynamic_cast<ColliderType*>(this);
			C78E_CORE_ASSERT(collider, "Collider::as: ColliderType& incompatible!");
			return *collider;
		}

		template<typename ColliderType>
		const ColliderType& as() const {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of<Collider<dim>, ColliderType>::value), "Collider::as: ColliderType incompatible(does not have Base Collider<dim>)!");
			const ColliderType* collider = dynamic_cast<const ColliderType*>(this);
			C78E_CORE_ASSERT(collider, "Collider::as: ColliderType& incompatible!");
			return *collider;
		}

	protected:
	};

}