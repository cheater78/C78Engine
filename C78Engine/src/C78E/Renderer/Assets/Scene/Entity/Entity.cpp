#include "C78EPCH.h"
#include "Entity.h"

#include "../Scene.h"

namespace C78E {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {
	}

}