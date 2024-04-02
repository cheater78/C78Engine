#pragma once
#include "C78EPCH.h"

#include "SceneCamera.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/UUID.h"
#include "C78E/Assets/AssetManager.h"

namespace C78E {
	// Forward declarations
	class Model;
	class Texture2D;
	class Font;
	class ScriptableEntity;

	//ComponentTypes

	// mandatory
	typedef UUID IDComponent;

	typedef std::string TagComponent;

	struct StateComponent {
		bool enable = true;
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 getTransform() const {
			return glm::translate(glm::mat4(1.0f), Translation)
				 * glm::toMat4(glm::quat(Rotation))
				 * glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 normalMat() {
			return glm::mat4(glm::inverseTranspose(getTransform()));
		}
	};

	// non-mandatory
	struct CameraComponent
	{
		SceneCamera Camera;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	

	// TODO -> Material
	typedef AmbientLight AmbientLightComponent;

	typedef DirectLight DirectLightComponent;

	typedef PointLight PointLightComponent;

	typedef SpotLight SpotLightComponent;
	// ----


	struct ModelComponent {
		std::vector<AssetHandle> models;
	};

	struct MeshComponent {
		AssetHandle mesh;
	};

	struct MaterialComponent {
		AssetHandle material;
	};

	struct TextureComponent {
		std::vector<AssetHandle> textures;
	};

	struct SkyBoxComponent {
		std::vector<AssetHandle> skyboxes;
	};


	struct SpriteRendererComponent {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		AssetHandle texture;
		float tilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CircleRendererComponent {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};


	struct ScriptComponent {
		std::string className;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};

	
	template<typename... Component>
	struct ComponentGroup
	{
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
		MeshComponent,
		MaterialComponent,
		TextureComponent,
		SkyBoxComponent,

		SpriteRendererComponent,
		CircleRendererComponent,
		CameraComponent,
		ScriptComponent,
		NativeScriptComponent
	>;
	
}