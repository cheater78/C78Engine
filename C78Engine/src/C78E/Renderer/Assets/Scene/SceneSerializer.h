#pragma once

#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Scene/Scene.h>
#include <C78E/Renderer/Assets/Scene/Entity/Entity.h>
#include <C78E/Utils/Yaml/YamlUtils.h>

namespace C78E {

	class SceneImporter {
	public:
		static Ref<Asset::Group> importScene(const FilePath& assetDirectory, Ref<Asset::Meta> meta, AssetHandle handle);
	};

	class SceneSerializer {
	public:
		public:
		static Ref<Scene> importScene(AssetHandle handle,const FilePath& filepath);
		static bool exportScene(Ref<Scene> scene, const FilePath& filepath);

		//Scene De-/Serialization
		static bool importEditorScene(Ref<Scene> scene, const std::string& serializedEditorScene);
		static bool importRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene);

		static bool exportEditorScene(Ref<Scene> scene, std::string& serializedEditorScene);
		static bool exportRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene);

	private:
		//Entity De-/Serialization
		static bool importEditorEntity(Ref<Scene> scene, Entity& entity, const YAML::Node& serializedEditorEntity);
		static bool importRuntimeEntity(Ref<Scene> scene, Entity& entity, Ref<ScopedBuffer> serializedRuntimeEnity);

		static bool exportEditorEntity(Ref<Scene> scene, Entity& entity, YAML::Emitter& serializedEditorEntity);
		static bool exportRuntimeEntity(Ref<Scene> scene, Entity& entity,  Ref<ScopedBuffer> serializedRuntimeEnity);
	};

}