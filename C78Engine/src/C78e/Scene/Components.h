#pragma once
#include "C78EPCH.h"

#include "SceneCamera.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/UUID.h"
#include "C78E/Assets/AssetManager.h"

namespace C78E {
	class ScriptableEntity;

	/*
	* #############################
	* Components for Scene Entities
	* #############################
	*/

	/*
	* ID Component, uniquely identifying an Entity
	* mandatory for every Entity
	* gets auto created on Entity creation
	*/
	struct IDComponent {
		UUID id;
	};

	/*
	* Tag Component, closer description of the Entity(e.g. name)
	* mandatory for every Entity
	*/
	struct TagComponent {
		std::string tag;
	};

	/*
	* State Component, whether an Entity is active or should be skipped in any handling
	* mandatory for every Entity
	* Entities are enabled by default
	*/
	struct StateComponent {
		bool enable = true;
	};

	/*
	* Transform Component, placement information of the Entity in virtual space
	* mandatory for every Entity
	*/
	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

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

	/*
	* Camera Component, containing data for the Entity to be used as a Camera
	* (optional)
	*/
	struct CameraComponent {
		SceneCamera Camera;
		bool FixedAspectRatio = false;
	};

	/*
	* Model Component, AssetHandles for registered Models to be rendered as visual representation of the Entity
	* (optional)
	*/
	struct ModelComponent {
		std::vector<AssetHandle> models;
	};

	/*
	* SkyBox Component, AssetHandles for registered Textures (TODO Cubemaps only?!) to be used as SkyBox for the Scene (TODO blending for multiple?!)
	* (optional)
	*/
	struct SkyBoxComponent {
		std::vector<AssetHandle> skyboxes;
	};


	//
	//
	// LEGACY -> TODO: rework in Material/Shader system
	//
	//

	// TODO -> Material
	typedef AmbientLight AmbientLightComponent;

	typedef DirectLight DirectLightComponent;

	typedef PointLight PointLightComponent;

	typedef SpotLight SpotLightComponent;
	// ----

	struct MeshComponent {
		AssetHandle mesh;
	};

	struct MaterialComponent {
		AssetHandle material;
	};

	struct TextureComponent {
		std::vector<AssetHandle> textures;
	};

	


	struct SpriteRendererComponent {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		AssetHandle texture;
		float tilingFactor = 1.0f;

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