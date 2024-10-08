#include "C78ePCH.h"
#include "FileManagerAssets.h"

#include <C78E/Renderer/Assets/Texture/TextureImporter.h>

namespace C78E::GUI {

	FileManagerAssets::FileManagerAssets(FilePath defaultIconPath) { //TODO: create and insert Default Icons

		m_UIcons.emplace(FileManagerIcon::FORWARD, C78E::TextureImporter::loadImageFile(defaultIconPath / "Forward.png"));
		m_UIcons.emplace(FileManagerIcon::BACKWARD, C78E::TextureImporter::loadImageFile(defaultIconPath / "Backward.png"));
		m_UIcons.emplace(FileManagerIcon::HOME, C78E::TextureImporter::loadImageFile(defaultIconPath / "Home.png"));
		m_UIcons.emplace(FileManagerIcon::PARENT, C78E::TextureImporter::loadImageFile(defaultIconPath / "Parent.png"));
		m_UIcons.emplace(FileManagerIcon::SEARCH, C78E::TextureImporter::loadImageFile(defaultIconPath / "Search.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::DIRECTORY, C78E::TextureImporter::loadImageFile(defaultIconPath / "Folder.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::PROJECT, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::SCENE, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::ASSETREGISTRY, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::MODEL, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::MESH, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::MATERIAL, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::SHADER, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		m_DefaultIcons.emplace(FileSystem::EntryType::IMAGE, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::FONT, C78E::TextureImporter::loadImageFile(defaultIconPath / "File.png"));
		
		m_DefaultIcons.emplace(FileSystem::EntryType::MISC, C78E::TextureImporter::loadImageFile(defaultIconPath / "FileUnknown.png"));
	}

	FileManagerAssets::~FileManagerAssets() { }

	Ref<Texture2D> FileManagerAssets::getIcon(FilePath file) {
		FilePathHash hash = std::hash<FilePath>()(file);
		FileSystem::EntryType type = FileSystem::getEntryType(file);

		auto it = m_CustomIcons.find(hash);
		if (it != m_CustomIcons.end() && it->second->isLoaded()) //TODO: load custom icons
			return it->second;
		else return m_DefaultIcons.at(type);
	}

}
