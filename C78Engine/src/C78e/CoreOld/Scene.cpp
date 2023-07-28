#include "C78ePCH.h"
#include "Scene.h"

#include "Camera.h"
#include "EntityComponents.h"
#include "UUID.h"

#include <stdexcept>

namespace C78e {
	Scene::Scene() {
	}
	Scene::~Scene() {
	}

	Entity Scene::createEntity(const std::string& name) {
		Entity entity{ m_EnttReg.create(), this };
		entity.addComponent<IDComponent>();
		entity.addComponent<TagComponent>(name);
		entity.addComponent<TransformComponent>();
		return entity;
	}
	
	Entity Scene::createEntity(UUID uuid, const std::string& name) {
		Entity entity{ m_EnttReg.create(), this };
		entity.addComponent<IDComponent>(uuid);
		entity.addComponent<TagComponent>(name);
		entity.addComponent<TransformComponent>();
		return entity;
	}
	
	Camera Scene::getActiveCam() {
		return Camera(m_ActiveCam, this );
	}

	Entity Scene::setActiveCam(Entity cam) {
		if (!cam.hasComponent<CameraComponent>()) throw std::runtime_error("Entity is not a camera");
		m_ActiveCam = cam.getEnttity();
		return cam;
	}

	Camera Scene::setActiveCam(Camera cam) {
		setActiveCam((Entity)cam);
		return cam;
	}

	UUID Scene::getUUID()
	{
		return m_ID;
	}

}
