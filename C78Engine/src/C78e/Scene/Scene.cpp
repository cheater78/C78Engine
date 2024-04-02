#include "C78EPCH.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>


namespace C78E {

	template<typename... Component>
	static void copyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap) {
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity));

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void copyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap) {
		copyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void copyComponentIfExists(Entity dst, Entity src) {
		([&]()
		{
			if (src.hasComponent<Component>())
				dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void copyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
		copyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::copy(Ref<Scene> other) {
		Ref<Scene> newScene = createRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView) {
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).id;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->createEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		copyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		newScene->m_ActiveCam = other->m_ActiveCam;

		return newScene;
	}

	Entity Scene::createEntity(const std::string& name) {
		return createEntityWithUUID(UUID(), name);
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.addComponent<IDComponent>(uuid);
		entity.addComponent<StateComponent>();
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>().tag;
		tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::destroyEntity(Entity entity) {
		m_EntityMap.erase(entity.getUUID());
		m_Registry.destroy(entity);
	}

	void Scene::onRuntimeStart() {
		m_IsRunning = true;
	}

	void Scene::onRuntimeStop() {
		m_IsRunning = false;
	}

	void Scene::onUpdateRuntime(Timestep ts) {
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height) {
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::setPrimaryCamera(Entity camera) {
		C78_CORE_ASSERT(camera.hasComponent<CameraComponent>(), "Primary Camera must have a CameraComponent!");
		C78_CORE_ASSERT(camera.isPartOf(this), "Primary Camera must be part of the Scene!");
		m_ActiveCam = camera.getUUID();
	}

	bool Scene::hasPrimaryCamera() { return (m_ActiveCam) ? false : Entity(m_EntityMap.at(m_ActiveCam), this); }

	Entity Scene::getPrimaryCamera() {
		C78_CORE_ASSERT(hasPrimaryCamera(), "Scene currently does not have an Active Cam!");
		return Entity(m_EntityMap.at(m_ActiveCam), this);
	}

	void Scene::step(int frames) {
		m_StepFrames = frames;
	}

	Entity Scene::duplicateEntity(Entity entity) {
		// Copy name because we're going to modify component data structure
		std::string name = entity.getTag();
		Entity newEntity = createEntity(name);
		copyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::findEntityByName(std::string_view name) {
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::getEntityByUUID(UUID uuid) {
		// TODO: Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	

	void Scene::renderScene(EditorCamera& camera) {
	}
  
	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component) {
		C78_CORE_WARN("Scene: onComponentAdded - missing Component, Entity: {}", entity.getTag());
	}

	template<>
	void Scene::onComponentAdded<IDComponent>(Entity entity, IDComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if (!m_ActiveCam)
			m_ActiveCam = entity.getUUID();
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::onComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<ModelComponent>(Entity entity, ModelComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<AmbientLightComponent>(Entity entity, AmbientLightComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<DirectLightComponent>(Entity entity, DirectLightComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<SpotLightComponent>(Entity entity, SpotLightComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {
	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
	}

}
