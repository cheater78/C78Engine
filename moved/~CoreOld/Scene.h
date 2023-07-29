#pragma once
#include <entt/entt.hpp>

#include "UUID.h"

#include <string>

namespace C78e {

	class Entity;
	class Camera;

	class Scene{
	public:
		
		Scene();
		//Scene(UUID id); //construct existing Scene
		Scene(const Scene&) = delete;
		~Scene();

		
		Entity createEntity(const std::string& name = std::string());
		Entity createEntity(UUID uuid, const std::string& name = std::string());

		Camera getActiveCam();
		Entity setActiveCam(Entity cam);
		Camera setActiveCam(Camera cam);

		UUID getUUID();

		entt::registry& reg() { return m_EnttReg; }
	private:
		entt::registry m_EnttReg;
		entt::entity m_ActiveCam{0};

		UUID m_ID;
	};
}



