#include "C78ePCH.h"
#include "TextureManager.h"

namespace C78e {
	Ref<TextureManager> TextureManager::s_MainTextureManager;

	TextureManager::TextureManager()
	{
	}

	TextureManager::~TextureManager()
	{
	}

	void TextureManager::load(std::string file)
	{
		m_Textures[file] = Texture2D::Create(file);
	}

	Ref<Texture2D> TextureManager::get(std::string file)
	{
		return m_Textures.at(file);
	}

	Ref<Texture2D> TextureManager::get_s(std::string file)
	{
		if (m_Textures.find(file) == m_Textures.end())
			load(file);
		return get(file);
	}

}