#pragma once
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Utils/StdUtils.h>
#include <C78E/Math/Math.h>

#include "Collision/Collider/Collider.h"

namespace C78E::Physics {

	/**
	 * @brief ColliderStorage, Memory for all Colliders, storing Colliders type agnostic
	 * @tparam dim 2D or 3D
	 */
	template<Dimension dim>
	class ColliderStorage {
	public:
		class ColliderType {
		public:
			enum Type {
				Plane, Sphere, AABB, OBB, Capsule, Cylinder, Hull, Mesh
			};
		public:
			ColliderType(Type type) : m_Type(type) { }
			ColliderType(const ColliderType& other) = default;
			~ColliderType() = default;

			bool operator==(const Type& other) const { return this->m_Type == other.m_Type; }
			bool operator==(uint8_t other) const { return this->m_Type == other; }
			bool operator!=(const Type& other) const { return this->m_Type != other.m_Type; }
			bool operator!=(uint8_t other) const { return this->m_Type != other; }

			static std::string colliderTypeToString(Type type); //TODO: needed?
			static Type colliderTypeFromString(const std::string& typeString); //TODO: needed?
		private:
			Type m_Type;
		};

	public:
		ColliderStorage() = delete;
		ColliderStorage(const PlaneCollider<dim>& planeCollider) :			m_Type(ColliderType::Plane),	m_InternalStorage(planeCollider			) { }
		ColliderStorage(const SphereCollider<dim>& sphereCollider) :		m_Type(ColliderType::Sphere),	m_InternalStorage(sphereCollider		) { }
		ColliderStorage(const AABBCollider<dim>& aABBCollider) :			m_Type(ColliderType::AABB),		m_InternalStorage(aABBCollider			) { }
		ColliderStorage(const CapsuleCollider<dim>& capsuleCollider) :		m_Type(ColliderType::Capsule),	m_InternalStorage(capsuleCollider		) { }
		ColliderStorage(const CylinderCollider<dim>& cylinderCollider) :	m_Type(ColliderType::Cylinder),	m_InternalStorage(cylinderCollider		) { }
		ColliderStorage(const HullCollider<dim>& hullCollider) :			m_Type(ColliderType::Hull),		m_InternalStorage(hullCollider			) { }
		ColliderStorage(const MeshCollider<dim>& meshCollider) :			m_Type(ColliderType::Mesh),		m_InternalStorage(meshCollider			) { }
		ColliderStorage(ColliderStorage& other)
			: m_Type(other.m_Type),
			m_InternalStorage(std::visit(
				[](const auto& value) -> InternalColliderStorage {
					return value;
				},
				other.m_InternalStorage
			)) {
		}
		~ColliderStorage() = default;

		ColliderType getType() const {
			return m_Type;
		}

		Collider<dim>* getColliderPtr() {
			Collider<dim>* ptr = nullptr;
			std::visit(
				[&](auto&& value) {
					ptr = (Collider<dim>*)&value;
				},
				m_InternalStorage
			);
			return ptr;
		}

		template<typename RequestedColliderType>
		RequestedColliderType& getColliderAs() {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of_v<Collider<dim>, RequestedColliderType>), "ColliderStorage::getColliderAs: RequestedColliderType is not of Basetype Collider!");
			C78E_CORE_ASSERT(std::holds_alternative<RequestedColliderType>(m_InternalStorage), "ColliderStorage::getColliderAs: RequestedColliderType is not of the current ColliderType!");
			return std::get<RequestedColliderType>(m_InternalStorage);
		}

		ColliderStorage<dim>& operator=(const ColliderStorage<dim>& other) {
			m_Type = other.m_Type;
			m_InternalStorage = std::visit(
				[](const auto& value) -> InternalColliderStorage {
					return value;
				},
				other.m_InternalStorage
			);
			return *this;
		}

	private:
		using InternalColliderStorage = std::variant<
			Physics::PlaneCollider<dim>,
			Physics::SphereCollider<dim>,
			Physics::AABBCollider<dim>,
			Physics::CapsuleCollider<dim>,
			Physics::CylinderCollider<dim>,
			Physics::HullCollider<dim>,
			Physics::MeshCollider<dim>
		>;

		ColliderType m_Type;
		InternalColliderStorage m_InternalStorage;
	};

}