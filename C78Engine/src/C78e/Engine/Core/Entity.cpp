#include "Entity.h"

#include "Scene.h"
#include "EntityComponents.h"
#include "UUID.h"

namespace C78e {

	Entity::Entity() {
	}

	Entity::Entity(entt::entity enttity, Scene* scene) : m_Enttity(enttity), m_Scene(scene) {
	}

	Entity::Entity(uint32_t enttID, Scene* scene) : m_Enttity((entt::entity)enttID), m_Scene(scene) {
	}

	
	Entity::~Entity() {
	}

	UUID Entity::getUUID() { return getComponent<IDComponent>().id; }

	void Entity::setTag(std::string tag) { auto& mtag = getComponent<TagComponent>().tag; mtag = tag; }

	std::string Entity::getTag() { return getComponent<TagComponent>().tag; }

	void Entity::setPos(glm::vec3 pos) { auto& mpos = getComponent<TransformComponent>().translation; mpos = pos; }

	glm::vec3 Entity::getPos() { return getComponent<TransformComponent>().translation; }

	entt::entity Entity::getEnttity() { return m_Enttity; }

	entt::registry& Entity::reg() { return m_Scene->reg(); }

}