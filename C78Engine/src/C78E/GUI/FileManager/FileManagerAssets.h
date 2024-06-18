#pragma once

#include <C78E/Core/FileSystem/FileSystem.h>
#include <C78E/Assets/Texture/Texture.h>
#include <C78E/GUI/ImGui/TextureHandle.h>

namespace C78E::GUI {

	enum FileManagerIcon {
		FORWARD,
		BACKWARD,
		HOME,
		PARENT,
		SEARCH
	};

	class FileManagerAssets {
	public:
		FileManagerAssets(FilePath defaultIconPath);
		FileManagerAssets(const FileManagerAssets& other) = delete;
		~FileManagerAssets();


		Ref<Texture2D> getIcon(FilePath file);
		GUI::TextureHandle getIconHandle(FilePath file) { return getIcon(file)->getRendererID(); }

		Ref<Texture2D> getUIcon(FileManagerIcon icon) const { return m_UIcons.at(icon); }
		GUI::TextureHandle getUIconHandle(FileManagerIcon icon) const { return getUIcon(icon)->getRendererID(); }

	private:
		std::unordered_map<FileManagerIcon, Ref<Texture2D>> m_UIcons;
		std::map<FileSystem::EntryType, Ref<Texture2D>> m_DefaultIcons;
		std::unordered_map<FilePathHash, Ref<Texture2D>> m_CustomIcons; // to AssetManager when there is one
	};

}
