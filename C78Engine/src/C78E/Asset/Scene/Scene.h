#pragma once
#include <entt/entt.hpp>
#include <C78E/Asset/Asset.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Renderer/Camera/EditorCamera.h>

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

		/**
		 * @brief
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam ...Components 
		 * @return 
		 */
		template<typename... Components>
		auto getAllEntitiesWith() { 
			return m_Registry.view<Components...>();
		}

		bool isEmpty() const;
		void forEachEntity(std::function<void(Entity entity)> func);

		/**
		 * @brief
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam Component 
		 * @tparam ...Components 
		 * @param func 
		 */
		template<typename Component, typename... Components>
		void forEachEntityComponent(std::function<void(Entity entity, Component&, Components& ...components)> func) {
			auto view = m_Registry.view<Component, Components...>();
			for (auto entity : view) {
				func(Entity{ entity, this }, view.get<Component>(entity), view.get<Components>(entity)...);
			}
		}

	public:
		virtual Type getType() const override { return Asset::Type::Scene; };
		static Type getClassType() { return Type::Scene; };
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void renderScene(EditorCamera& camera);
	private:
		// Entities
		entt::registry m_Registry;
		UUID m_ActiveCam = UUID::invalid();
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
