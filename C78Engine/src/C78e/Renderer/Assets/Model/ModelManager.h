#pragma once
#include "C78E/Renderer/Assets/Model/Mesh.h"

namespace C78E {
	class ModelManager {
	public:
		ModelManager();
		~ModelManager();

		void load(std::string file);
		void load(std::string name, Mesh mesh);
		Ref<Mesh> get(std::string name);

		Ref<Mesh> get_s(std::string file);


		static Ref<ModelManager> get() {
			if (!s_MainTextureManager)
				s_MainTextureManager = createRef<ModelManager>();
			return s_MainTextureManager;
		}

	private:
		std::unordered_map<std::string, Ref<Mesh>> m_Textures;


		static Ref<ModelManager> s_MainTextureManager;
	};
}