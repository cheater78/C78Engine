#pragma once
#include <C78E/Core/UUID.h>
#include <C78E/Renderer/Camera/SceneCamera.h>
#include <C78E/Asset/Asset.h>
#include <C78E/Asset/Model/Model.h>

#include "Uniform.h"

#include <C78E/Asset/Font/Font.h>

namespace C78E {
	class ScriptableEntity;

	/*
	* #############################
	* Components for Scene Entities
	* #############################
	*/

	/**
	 * @brief ID Component, uniquely identifying an Entity
	 * mandatory for every Entity
	 * gets auto created on Entity creation
	 */
	struct IDComponent {
		IDComponent(UUID _id);
		UUID id;
	};

	/**
	 * @brief Tag Component, closer description of the Entity (e.g. name)
	 * mandatory for every Entity
	 */
	struct TagComponent {
		std::string tag;
	};

	/**
	 * @brief State Component, whether an Entity is active or should be skipped in any handling
	 * mandatory for every Entity
	 * Entities are enabled by default
	 */
	struct StateComponent {
		bool enable = true;
	};

	/**
	 * @brief Transform Component, placement information of the Entity in virtual space
	 * mandatory for every Entity
	 */
	struct TransformComponent {
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation);

		glm::mat4 getTransform() const;
		glm::mat4 normalMat();
	};

	/**
	 * @brief Camera Component, containing data for the Entity to be used as a Camera
	 * (optional)
	 */
	struct CameraComponent {
		SceneCamera camera;
		bool fixedAspectRatio = false;
	};

	/**
	 * @brief Model Component, AssetHandles for registered Models to be rendered as visual representation of the Entity
	 * (optional)
	 */
	struct ModelComponent {
		AssetHandle model;
	};

	/**
	 * @brief SkyBox Component, AssetHandle for registered Texture to be used as SkyBox for the Scene
	 * (optional)
	 */
	struct SkyBoxComponent {
		AssetHandle skybox;
	};

	// Legacy components
	typedef Light::AmbientLight AmbientLightComponent;
	typedef Light::DirectLight DirectLightComponent;
	typedef Light::PointLight PointLightComponent;
	typedef Light::SpotLight SpotLightComponent;

	struct SpriteRendererComponent {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		AssetHandle texture = AssetHandle::invalid();
		float tilingFactor = 1.0f;
	};

	struct CircleRendererComponent {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct TextComponent {
		std::string textString;
		AssetHandle fontAsset;
		glm::vec4 color{ 1.0f };
		float kerning = 0.0f;
		float lineSpacing = 0.0f;
	};

	struct ScriptComponent {
		std::string className;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind();
	};

	template<typename... Component>
	struct ComponentGroup {
	};

	using AllComponents = ComponentGroup <
		IDComponent,
		TagComponent,
		StateComponent,
		TransformComponent,
		CameraComponent,
		AmbientLightComponent,
		DirectLightComponent,
		PointLightComponent,
		SpotLightComponent,
		ModelComponent,
		SkyBoxComponent,
		SpriteRendererComponent,
		CircleRendererComponent,
		TextComponent,
		ScriptComponent,
		NativeScriptComponent
	>;
}