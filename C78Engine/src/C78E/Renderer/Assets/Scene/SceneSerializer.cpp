#include <C78ePCH.h>
#include "SceneSerializer.h"



namespace C78E {

	SceneSerializer::SceneSerializer(const Ref<Scene> scene, const Asset::AssetMeta& meta)
		: m_Scene(scene), m_Meta(meta)
	{ }

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		C78_CORE_ASSERT(entity.hasComponent<IDComponent>());

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

		if (entity.hasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.hasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.hasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.hasComponent<ModelComponent>()) {
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap; // ModelComponent

			auto& modelComponent = entity.getComponent<ModelComponent>();
			auto& model = modelComponent.model;

			out << YAML::Key << "Model" << YAML::Value << model;

			out << YAML::EndMap; // ModelComponent
		}

		if (entity.hasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteComponent = entity.getComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
			out << YAML::Key << "Texture" << YAML::Value << spriteComponent.texture;
			out << YAML::Key << "Tiling" << YAML::Value << spriteComponent.tilingFactor;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.hasComponent<TextComponent>()) {
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap; // TextComponent

			auto& spriteComponent = entity.getComponent<TextComponent>();
			out << YAML::Key << "DisplayText" << YAML::Value << spriteComponent.textString;
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
			out << YAML::Key << "Font" << YAML::Value << spriteComponent.fontAsset;
			out << YAML::Key << "Kerning" << YAML::Value << spriteComponent.kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << spriteComponent.lineSpacing;
			
			out << YAML::EndMap; // TextComponent
		}


		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::serialize() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Meta.name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->forEachEntity(
			[&](auto entityID) {
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;
				SerializeEntity(out, entity);
			}
		);
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(m_Meta.fileSource);
		fout << out.c_str();
	}

	void SceneSerializer::serializeRuntime(const FilePath& filepath) {
		// Not implemented
		C78_CORE_ASSERT(false, "RuntimeSerialization of Scene is not implemented!");
	}

	bool SceneSerializer::deserialize(const FilePath& filepath) {
		YAML::Node data;
		try {
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e) {
			C78_CORE_ERROR("Failed to load .sce file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		
		m_Meta.name = data["Scene"].as<std::string>();
		m_Meta.fileSource = filepath;
		m_Meta.type = Asset::AssetType::Scene;
		C78_CORE_TRACE("SceneSerializer::deserialize: Deserializing scene '{0}'", m_Meta.name);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				UUID uuid = entity["Entity"].as<UUID>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				

				Entity deserializedEntity = m_Scene->createEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent) {
					// Entities always have transforms
					auto& tc = deserializedEntity.getComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = deserializedEntity.addComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto modelComponent = entity["ModelComponent"];
				if (modelComponent) {
					auto& mc = deserializedEntity.addComponent<ModelComponent>();

					auto model = modelComponent["Model"];
					mc.model = model.as<UUID>();
				}

				auto spriteComponent = entity["SpriteRendererComponent"];
				if (spriteComponent) {
					auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteComponent["Color"].as<glm::vec4>();
					src.texture = spriteComponent["Texture"].as<AssetHandle>();
					src.tilingFactor = spriteComponent["Tiling"].as<float>();
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent) {
					auto& src = deserializedEntity.addComponent<TextComponent>();
					src.textString = textComponent["DisplayText"].as<std::string>();
					src.color = textComponent["Color"].as<glm::vec4>();
					src.fontAsset = textComponent["Font"].as<AssetHandle>();
					src.kerning = textComponent["Kerning"].as<float>();
					src.lineSpacing = textComponent["LineSpacing"].as<float>();
				}


				C78_CORE_TRACE("SceneSerializer::deserialize: Deserialized entity with ID = {0}, name = {1}", uuid, name);
			}
		}

		return true;
	}

	bool SceneSerializer::deserializeRuntime(const FilePath& filepath) {
		// Not implemented
		C78_CORE_ASSERT(false, "RuntimeDeserialization of Scene is not implemented!");
		return false;
	}

}


