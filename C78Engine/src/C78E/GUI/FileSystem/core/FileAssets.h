#pragma once

#include <C78E/Core/FileSystem.h>
#include <C78E/Assets/Texture/Texture.h>
#include <C78E/ImGui/TextureHandle.h>

#include "imgui.h"

namespace C78E {

	enum FileManagerIcon {
		FORWARD,
		BACKWARD,
		HOME,
		PARENT,
	};

	class FileAssets {
	public:
		FileAssets(FilePath defaultIconPath);
		FileAssets(const FileAssets& other) = delete;
		~FileAssets();


		Ref<Texture2D> getIcon(FilePath file);
		TextureHandle getIconHandle(FilePath file) { return getIcon(file)->getRendererID(); }

		Ref<Texture2D> getUIcon(FileManagerIcon icon) const { return m_UIcons.at(icon); }
		TextureHandle getUIconHandle(FileManagerIcon icon) const { return getUIcon(icon)->getRendererID(); }

	private:
		std::unordered_map<FileManagerIcon, Ref<Texture2D>> m_UIcons;
		std::map<FileSystem::EntryType, Ref<Texture2D>> m_DefaultIcons;
		std::unordered_map<FilePathHash, Ref<Texture2D>> m_CustomIcons;
		

	};

}
