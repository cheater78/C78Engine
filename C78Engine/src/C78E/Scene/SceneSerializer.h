#pragma once
#include "C78E/Scene/Scene.h"
#include "C78E/Scene/Entity.h"

//pre decl
namespace YAML {
	class Emitter;
}

namespace C78E {

	class SceneSerializer {
	public:
		SceneSerializer() = delete;
		SceneSerializer(const Ref<Scene>& scene);
		SceneSerializer(const SceneSerializer& other) = delete;
		~SceneSerializer() = default;

		void serialize(const FilePath& filepath);
		void serializeRuntime(const FilePath& filepath);

		bool deserialize(const FilePath& filepath);
		bool deserializeRuntime(const FilePath& filepath);
	private:
		Ref<Scene> m_Scene;
	};

}
