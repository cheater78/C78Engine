#pragma once

#include "C78e/Core/Timestep.h"
#include "C78e/Core/UUID.h"
#include "C78e/Renderer/EditorCamera.h"



namespace C78e {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> copy(Ref<Scene> other);

		Entity createEntity(const std::string& name = std::string());
		Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onRuntimeStart();
		void onRuntimeStop();

		void onUpdateRuntime(Timestep ts);
		void onViewportResize(uint32_t width, uint32_t height);

		Entity duplicateEntity(Entity entity);

		Entity findEntityByName(std::string_view name);
		Entity getEntityByUUID(UUID uuid);

		Entity getPrimaryCameraEntity();

		bool isRunning() const { return m_IsRunning; }
		bool isPaused() const { return m_IsPaused; }

		void setPaused(bool paused) { m_IsPaused = paused; }

		void step(int frames = 1);

		template<typename... Components>
		auto getAllEntitiesWith() { return m_Registry.view<Components...>(); }

	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void renderScene(EditorCamera& camera);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
