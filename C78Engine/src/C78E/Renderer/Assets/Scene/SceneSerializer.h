#pragma once
#include <C78E/Renderer/Assets/Scene/Scene.h>
#include <C78E/Renderer/Assets/Scene/Entity/Entity.h>

#include <C78E/Utils/Yaml/YamlUtils.h>

namespace C78E {

	class SceneSerializer {
	public:
		SceneSerializer() = delete;
		SceneSerializer(const Ref<Scene> scene, const Asset::AssetMeta& meta);
		SceneSerializer(const SceneSerializer& other) = delete;
		~SceneSerializer() = default;

		void serialize();
		void serializeRuntime(const FilePath& filepath);

		bool deserialize(const FilePath& filepath);
		bool deserializeRuntime(const FilePath& filepath);
	private:
		Ref<Scene> m_Scene;
		Asset::AssetMeta m_Meta;
	};

}
