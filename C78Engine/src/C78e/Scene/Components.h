#pragma once
#include "C78EPCH.h"

#include "SceneCamera.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/UUID.h"
#include "C78E/Renderer/Font.h"
#include "C78E/Renderer/Assets/Model/Mesh.h"
#include "C78E/Renderer/Assets/Material/Material.h"

namespace C78E {
	// Forward declaration
	class Model;
	class Texture2D;
	class Font;
	class ScriptableEntity;

	//ComponentTypes

	typedef UUID IDComponent;

	typedef std::string TagComponent;

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

		glm::mat4 TransformComponent::normalMat() {
			return glm::mat4(glm::inverseTranspose(getTransform()));
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	
	typedef AmbientLight AmbientLightComponent;

	typedef DirectLight DirectLightComponent;

	typedef PointLight PointLightComponent;

	typedef SpotLight SpotLightComponent;
	
	struct MeshComponent {
		Ref<Mesh> mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(std::vector<Vertex> vertecies, std::vector<uint32_t> indicies)
			: mesh(createRef<Mesh>(vertecies, indicies))
		{}
		MeshComponent(const char* file)
			: mesh(createRef<Mesh>(file))
		{}
	};

	typedef Material MaterialComponent;





	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};


	struct ScriptComponent
	{
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct TextComponent
	{
		std::string TextString;
		Ref<Font> FontAsset = Font::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
	};

	
	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup <
		IDComponent,
		TagComponent,
		TransformComponent,

		CameraComponent,

		AmbientLightComponent,
		DirectLightComponent,
		PointLightComponent,
		SpotLightComponent,

		MeshComponent,
		MaterialComponent,

		SpriteRendererComponent,
		CircleRendererComponent,
		CameraComponent,
		ScriptComponent,
		NativeScriptComponent,
		TextComponent
	>;
	
}