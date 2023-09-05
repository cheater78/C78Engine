#include "C78ePCH.h"
#include "Entity.h"

namespace C78e {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}