#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include <stdexcept>

namespace C78e {

	class Scene;
	class UUID;

	class Entity {
	public:

		Entity();
		Entity(entt::entity enttity, Scene* scene);
		Entity(uint32_t enttID, Scene* scene);
		Entity(const Entity&) = default;
		~Entity();

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {		if(hasComponent<T>()) throw std::runtime_error("Entity already has this Component");
												return reg().emplace<T>(m_Enttity, std::forward<Args>(args)...); }

		template<typename T>
		T& getComponent() {						if (!hasComponent<T>()) throw std::runtime_error("Entity does not have this Component");
												return reg().get<T>(m_Enttity); }

		template<typename T>
		bool hasComponent() {					return reg().all_of<T>(m_Enttity); }

		template<typename T>
		void removeComponent() {				if (!hasComponent<T>()) throw std::runtime_error("Entity does not have this Component");
												reg().remove<T>(m_Enttity); }

		UUID getUUID();
		void setTag(std::string tag);
		std::string getTag();
		void setPos(glm::vec3 pos);
		glm::vec3 getPos();


		entt::entity getEnttity();
		
		operator bool() const { return (uint32_t)m_Enttity != 0; }
	private:
		entt::entity m_Enttity{ entt::null };
		Scene* m_Scene = nullptr;

		entt::registry& reg();
	};
}



