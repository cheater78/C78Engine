#pragma once

#include "C78E/Core/UUID.h"
#include "C78E/Core/Timestep.h"
#include "C78E/Renderer/Camera/EditorCamera.h"

#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	class Entity;
	using SceneHandle = AssetHandle;
	class Scene : public Asset {
	public:
		static Ref<Scene> copy(Ref<Scene> other);

	public:
		Scene() = default;
		~Scene() = default;

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

		void setPrimaryCamera(Entity camera);
		bool hasPrimaryCamera();
		Entity getPrimaryCamera();

		bool isRunning() const { return m_IsRunning; }
		bool isPaused() const { return m_IsPaused; }

		void setPaused(bool paused) { m_IsPaused = paused; }

		void step(int frames = 1);

		template<typename... Components>
		auto getAllEntitiesWith() { return m_Registry.view<Components...>(); }

		bool isEmpty() const { return m_Registry.empty(); }

		using EntityFunction = std::function<void(entt::entity)>;
		void forEachEntity(EntityFunction func);

	public:
		virtual AssetType getType() const override { return Asset::AssetType::Scene; };
		static AssetType getClassType() { return AssetType::Scene; };
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void renderScene(EditorCamera& camera);
	private:
		// Entities
		entt::registry m_Registry;
		UUID m_ActiveCam{ 0 };
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		// Runtime
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		friend class Entity;
		friend class SceneSerializer;
	};

}
