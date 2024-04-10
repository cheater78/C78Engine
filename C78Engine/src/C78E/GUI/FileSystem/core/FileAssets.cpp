#include "C78ePCH.h"
#include "FileAssets.h"
#include <C78E/Assets/Texture/TextureLoader.h>

namespace C78E {

	FileAssets::FileAssets(FilePath defaultIconPath) { //TODO: create and insert Default Icons

		m_UIcons.emplace(FileManagerIcon::FORWARD, C78E::TextureLoader::loadTexture2D(defaultIconPath / "Forward.png"));
		m_UIcons.emplace(FileManagerIcon::BACKWARD, C78E::TextureLoader::loadTexture2D(defaultIconPath / "Backward.png"));
		m_UIcons.emplace(FileManagerIcon::HOME, C78E::TextureLoader::loadTexture2D(defaultIconPath / "Home.png"));
		m_UIcons.emplace(FileManagerIcon::PARENT, C78E::TextureLoader::loadTexture2D(defaultIconPath / "Parent.png"));


		m_DefaultIcons.emplace(FileSystem::EntryType::DIRECTORY, C78E::TextureLoader::loadTexture2D(defaultIconPath / "Folder.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::PROJECT, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::SCENE, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::ASSETREGISTRY, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::MODEL, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::MESH, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::MATERIAL, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));

		m_DefaultIcons.emplace(FileSystem::EntryType::SHADER, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::IMAGE, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));

		m_DefaultIcons.emplace(FileSystem::EntryType::FONT, C78E::TextureLoader::loadTexture2D(defaultIconPath / "File.png"));

		m_DefaultIcons.emplace(FileSystem::EntryType::MISC, C78E::TextureLoader::loadTexture2D(defaultIconPath / "FileUnknown.png"));
	}

	FileAssets::~FileAssets() { }

	Ref<Texture2D> FileAssets::getIcon(FilePath file) {
		FilePathHash hash = std::hash<FilePath>()(file);
		FileSystem::EntryType type = FileSystem::getEntryType(file);

		auto it = m_CustomIcons.find(hash);
		if (it != m_CustomIcons.end() && it->second->isLoaded()) //TODO: load custom icons
			return it->second;
		else return m_DefaultIcons.at(type);
	}

}
