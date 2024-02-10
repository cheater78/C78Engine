#pragma once
#include "C78E/Core/Core.h"
#include "Texture.h"

namespace C78E {
	class TextureManager {
	public:

		static Ref<TextureManager> create() { s_TextureManager = createRef<TextureManager>(); return s_TextureManager; }
		static Ref<TextureManager> get() { if (!s_TextureManager) return create(); else return s_TextureManager; }

		TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		~TextureManager() = default;

		void add(const std::string& name, const Ref<Texture2D>& texture);
		void add(const Ref<Texture2D>& texture);
		Ref<Texture2D> load(const std::string& filepath);
		Ref<Texture2D> load(const std::string& name, const std::string& filepath);

		Ref<Texture2D> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;

		//Singleton TODO: change
		static Ref<TextureManager> s_TextureManager;
	};
}