#pragma once
#include "C78E/Core/Core.h"
#include "Texture.h"

namespace C78E {
	class TextureManager {
	public:
		TextureManager();
		~TextureManager();

		void load(std::string file);
		Ref<Texture2D> get(std::string file);
		Ref<Texture2D> get_s(std::string file);

		static Ref<TextureManager> get() {
			if (!s_MainTextureManager)
				s_MainTextureManager = createRef<TextureManager>();
			return s_MainTextureManager;
		}

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;


		static Ref<TextureManager> s_MainTextureManager;
	};
}