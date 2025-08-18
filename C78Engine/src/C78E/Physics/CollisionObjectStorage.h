#pragma once
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Utils/StdUtils.h>
#include <C78E/Math/Math.h>

#include <C78E/Physics/CollisionObject.h>

namespace C78E::Physics {

	/**
	 * @brief CollisionObjectStorage, Memory for all CollisionObjects, storing CollisionObjects type agnostic
	 * @tparam dim 2D or 3D
	 */
	template<Dimension dim>
	class CollisionObjectStorage {
	public:
		class CollisionObjectType {
		public:
			enum Type {
				Static, Dynamic
			};
		public:
			CollisionObjectType(Type type) : m_Type(type) { }
			CollisionObjectType(const CollisionObjectType& other) = default;
			~CollisionObjectType() = default;

			bool operator==(const Type& other) const { return this->m_Type == other.m_Type; }
			bool operator==(uint8_t other) const { return this->m_Type == other; }
			bool operator!=(const Type& other) const { return this->m_Type != other.m_Type; }
			bool operator!=(uint8_t other) const { return this->m_Type != other; }

			static std::string collisionObjectTypeToString(Type type); //TODO: needed?
			static Type collisionObjectTypeFromString(const std::string& typeString); //TODO: needed?
		private:
			Type m_Type;
		};

	public:
		CollisionObjectStorage() = delete;
		CollisionObjectStorage(const Physics::CollisionObject<dim>& collisionObject) : m_Type(CollisionObjectType::Static), m_InternalStorage(collisionObject) { }
		CollisionObjectStorage(const Physics::RigidBody<dim>& rigidBody) : m_Type(CollisionObjectType::Dynamic), m_InternalStorage(rigidBody) { }
		CollisionObjectStorage(CollisionObjectStorage& other)
			: m_Type(other.m_Type), 
			m_InternalStorage(std::visit(
				[](const auto& value) -> InternalCollisionObjectStorage {
					return value;
				},
				other.m_InternalStorage
			)) {
		}
		~CollisionObjectStorage() = default;

		CollisionObjectType getType() const {
			return m_Type;
		}

		template<typename RequestedCollisionObjectType>
		RequestedCollisionObjectType& getColliderObjectAs() {
			C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO(std::is_base_of_v<CollisionObject<dim>, RequestedCollisionObjectType>), "CollisionObjectStorage::getColliderObjectAs: RequestedCollisionObjectType is not of Basetype CollisionObject!");
			C78E_CORE_ASSERT(std::holds_alternative<RequestedCollisionObjectType>(m_InternalStorage), "CollisionObjectStorage::getColliderObjectAs: RequestedCollisionObjectType is not of the current CollisionObjectType!");
			return std::get<RequestedCollisionObjectType>(m_InternalStorage);
		}

		CollisionObject<dim>* getCollisionObjectPtr() {
			CollisionObject<dim>* ptr = nullptr;
			std::visit(
				[&](auto&& value) {
					ptr = (CollisionObject<dim>*) & value;
				},
				m_InternalStorage
			);
			return ptr;
		}

		CollisionObjectStorage<dim>& operator=(const CollisionObjectStorage<dim>& other) {
			m_Type = other.m_Type;
			m_InternalStorage = std::visit(
				[](const auto& value) -> InternalCollisionObjectStorage {
					return value;
				},
				other.m_InternalStorage
			);
			return *this;
		}

	private:
		using InternalCollisionObjectStorage = std::variant<
			Physics::CollisionObject<dim>,
			Physics::RigidBody<dim>
		>;

		CollisionObjectType m_Type;
		InternalCollisionObjectStorage m_InternalStorage;
	};
}
