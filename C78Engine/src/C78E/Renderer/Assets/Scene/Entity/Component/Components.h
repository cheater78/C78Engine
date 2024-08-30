#pragma once
#include "C78ePCH.h"

#include <C78E/Core/UUID.h>
#include <C78E/Renderer/Camera/SceneCamera.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Scene/Entity/Component/Light.h>

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
		IDComponent(UUID _id) : id(_id) {} //maybe needed
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

		TransformComponent() = default;
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
		SceneCamera camera;
		bool fixedAspectRatio = false;
	};

	/*
	* Model Component, AssetHandles for registered Models to be rendered as visual representation of the Entity
	* (optional)
	*/
	struct ModelComponent {
		AssetHandle model;
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
	typedef Light::AmbientLight AmbientLightComponent;

	typedef Light::DirectLight DirectLightComponent;

	typedef Light::PointLight PointLightComponent;

	typedef Light::SpotLight SpotLightComponent;
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
		AssetHandle texture = 0;
		float tilingFactor = 1.0f;
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
		void bind() {
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