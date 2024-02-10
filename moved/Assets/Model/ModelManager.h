#pragma once
#include "C78E/Renderer/Assets/Model/Mesh.h"

namespace C78E {
	class ModelManager {
	public:
		static Ref<ModelManager> create() { s_ModelManager = createRef<ModelManager>(); return s_ModelManager; }
		static Ref<ModelManager> get() { if (!s_ModelManager) return create(); else return s_ModelManager; }

		ModelManager() = default;
		ModelManager(const ModelManager&) = delete;
		~ModelManager() = default;

		void add(const std::string& name, const Ref<Mesh>& mesh);
		void add(const Ref<Mesh>& mesh);
		Ref<Mesh> load(const std::string& filepath);
		Ref<Mesh> load(const std::string& name, const std::string& filepath);
		Ref<Mesh> load(const std::string& name, Mesh mesh);

		Ref<Mesh> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Mesh>> m_Meshes;

		//Singleton TODO: change
		static Ref<ModelManager> s_ModelManager;
	};
}