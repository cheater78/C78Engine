#include "C78EPCH.h"
#include "Entity.h"
#include "Component/Components.h"

namespace C78E {

	Entity::Entity()
		: m_EntityHandle(entt::null), m_Scene(nullptr) {
	}

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {
	}

	Entity::Entity(const Entity& other)
		: m_EntityHandle(other.m_EntityHandle), m_Scene(other.m_Scene) {
	}

	Entity::~Entity() {
	}

	bool Entity::isPartOf(Scene* scene) {
		return m_Scene == scene;
	}

	Entity::operator bool() const {
		return m_EntityHandle != entt::null;
	}

	Entity::operator entt::entity() const {
		return m_EntityHandle;
	}

	Entity::operator uint32_t() const {
		return static_cast<uint32_t>(m_EntityHandle);
	}

	UUID Entity::getUUID() {
		return getComponent<IDComponent>().id;
	}

	std::string& Entity::getTag() {
		return getComponent<TagComponent>().tag;
	}

	TransformComponent& Entity::getTransform() {
		return getComponent<TransformComponent>();
	}

	TransformComponent& Entity::setTransform(glm::vec3 translation) {
		auto& transform = getTransform();
		transform.translation = translation;
		return transform;
	}

	TransformComponent& Entity::setTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
		auto& transform = getTransform();
		transform.translation = translation;
		transform.rotation = rotation;
		transform.scale = scale;
		return transform; }

	TransformComponent& Entity::transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
		auto& transform = getTransform();
		transform.translation += translate;
		transform.rotation += rotate;
		transform.scale *= scale;
		return transform;
	}

	bool Entity::operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
	bool Entity::operator!=(const Entity& other) const { return !(*this == other); }

}