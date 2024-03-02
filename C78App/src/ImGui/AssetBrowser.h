#pragma once
#include <C78e.h>

namespace C78Editor {

	class AssetBrowser {
	private:
		enum AssetType {
			Shader,
			Model,
			Mesh,
			Material,
			Texture
		};

	public:
		static void init();
		static void onImGuiRender();

	private:

	private:
		static bool s_ShowAssetBrowser;


		static constexpr size_t s_MaxTextInputSize = 1024;
		static char s_NewAssetFileInput[s_MaxTextInputSize];
		static size_t s_NewAssetFileInputBufferSize;
		static char s_NewAssetNameInput[s_MaxTextInputSize];
		static size_t s_NewAssetNameInputBufferSize;

		static AssetType s_AssetBrowserActiveTab;
	};
}