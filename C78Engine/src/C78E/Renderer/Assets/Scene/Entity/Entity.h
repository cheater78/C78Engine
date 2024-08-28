#pragma once
#include "Component/Components.h"
#include "../Scene.h"

namespace C78E {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			C78_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->onComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->onComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& getComponent() {
			C78_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool hasComponent() {
			C78_ASSERT(m_Scene->m_Registry.valid(m_EntityHandle), "Entity is not valid");
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void removeComponent() {
			C78_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool isPartOf(Scene* scene) { return m_Scene == scene; }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID getUUID() { return getComponent<IDComponent>().id; }
		const std::string& getTag() { return getComponent<TagComponent>().tag; }

		TransformComponent& getTransform() { return getComponent<TransformComponent>(); }
		TransformComponent& setTransform(glm::vec3 translation) { auto& transform = getTransform(); transform.Translation = translation; return transform; }
		TransformComponent& setTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f)) { auto& transform = getTransform(); transform.Translation = translation; transform.Rotation = rotation; transform.Scale = scale; return transform; }
		TransformComponent& transform(glm::vec3 translate, glm::vec3 rotate = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f)) { auto& transform = getTransform(); transform.Translation += translate; transform.Rotation += rotate; transform.Scale *= scale; return transform;	}

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
