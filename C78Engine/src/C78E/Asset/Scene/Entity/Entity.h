#pragma once
#include <C78E/Core/UUID.h>
#include <C78E/Asset/Scene/Scene.h>

namespace C78E {
	struct TransformComponent;

	/**
	 * @brief View of an entity in a Scene, providing access to its Components
	 * creatable, copyable, deletable light weight handle
	 */
	class Entity {
	public:
		/**
		 * @brief 
		 */
		Entity();

		/**
		 * @brief 
		 * @param handle 
		 * @param scene 
		 */
		Entity(entt::entity handle, Scene* scene);

		/**
		 * @brief 
		 * @param other 
		 */
		Entity(const Entity& other);

		/**
		 * @brief 
		 */
		~Entity();

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @tparam ...Args 
		 * @param ...args 
		 * @return 
		 */
		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			C78E_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->onComponentAdded<T>(*this, component);
			return component;
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @tparam ...Args 
		 * @param ...args 
		 * @return 
		 */
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->onComponentAdded<T>(*this, component);
			return component;
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @return 
		 */
		template<typename T>
		T& getComponent() {
			C78E_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @return 
		 */
		template<typename T>
		bool hasComponent() {
			C78E_ASSERT(m_Scene->m_Registry.valid(m_EntityHandle), "Entity is not valid");
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 */
		template<typename T>
		void removeComponent() {
			C78E_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		/**
		 * @brief 
		 * @param scene 
		 * @return 
		 */
		bool isPartOf(Scene* scene);

		explicit operator bool() const;
		operator entt::entity() const;
		operator uint32_t() const;
		
		/**
		 * @brief 
		 * @return 
		 */
		UUID getUUID();

		/**
		 * @brief 
		 * @return 
		 */
		std::string& getTag();

		/**
		 * @brief 
		 * @return 
		 */
		TransformComponent& getTransform();

		/**
		 * @brief 
		 * @param translation 
		 * @return 
		 */
		TransformComponent& setTransform(glm::vec3 translation);

		/**
		 * @brief 
		 * @param translation 
		 * @param rotation 
		 * @param scale 
		 * @return 
		 */
		TransformComponent& setTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));

		/**
		 * @brief 
		 * @param translate 
		 * @param rotate 
		 * @param scale 
		 * @return 
		 */
		TransformComponent& transform(glm::vec3 translate, glm::vec3 rotate = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));

		bool operator==(const Entity& other) const;
		bool operator!=(const Entity& other) const;
	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene;
	};
}
