#include "C78EPCH.h"
#include "TextureManager.h"

namespace C78E {
	Ref<TextureManager> TextureManager::s_TextureManager;

	void TextureManager::add(const std::string& name, const Ref<Texture2D>& texture)
	{
		C78_CORE_ASSERT(!exists(name), "Texture already exists!");
		m_Textures[name] = texture;
	}

	void TextureManager::add(const Ref<Texture2D>& texture)
	{
		auto& name = texture->getName();
		add(name, texture);
	}

	Ref<Texture2D> TextureManager::load(const std::string& filepath)
	{
		auto shader = Texture2D::create(filepath);
		add(shader);
		return shader;
	}

	Ref<Texture2D> TextureManager::load(const std::string& name, const std::string& filepath)
	{
		auto texture = Texture2D::create(filepath);
		add(name, texture);
		return texture;
	}

	Ref<Texture2D> TextureManager::get(const std::string& name)
	{
		C78_CORE_ASSERT(exists(name), "Texture not found!");
		return m_Textures[name];
	}

	bool TextureManager::exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}

}