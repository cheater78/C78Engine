#pragma once
#include "C78EPCH.h"

#include "SceneCamera.h"
#include "C78E/Core/Types.h"
#include "C78E/Core/UUID.h"
#include "C78E/Renderer/Font.h"
#include "C78E/Renderer/Assets/Model/Mesh.h"

namespace C78E {
	// Forward declaration
	class Model;
	class Texture2D;
	class Font;
	class ScriptableEntity;

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
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

	struct AmbientLightComponent {
		AmbientLight ambientLight{};

		AmbientLightComponent() = default;
		AmbientLightComponent(const AmbientLightComponent&) = default;
		AmbientLightComponent(AmbientLight ambientLight)
			: ambientLight{ ambientLight }
		{}
	};

	struct DirectLightComponent {
		DirectLight directLight{};

		DirectLightComponent() = default;
		DirectLightComponent(const DirectLightComponent&) = default;
		DirectLightComponent(DirectLight directLight)
			: directLight{ directLight }
		{}
	};

	struct PointLightComponent {
		PointLight pointLight{};

		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(PointLight pointLight)
			: pointLight{ pointLight }
		{}
	};

	struct SpotLightComponent {
		SpotLight spotLight{};

		SpotLightComponent() = default;
		SpotLightComponent(const SpotLightComponent&) = default;
		SpotLightComponent(SpotLight spotLight)
			: spotLight{ spotLight }
		{}
	};







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

	using AllComponents = 
		ComponentGroup<TransformComponent, SpriteRendererComponent,
			CircleRendererComponent, CameraComponent, ScriptComponent,
			NativeScriptComponent, TextComponent>;

}
