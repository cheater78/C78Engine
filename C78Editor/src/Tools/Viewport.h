#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class Viewport {
	public:

		static void init();

		static void onImGuiRender(bool& captureMouse, C78E::Ref<C78E::Renderer> renderer);

		static glm::vec<2, uint32_t> getSize() { return { s_Width, s_Height }; }

	private:
		static C78E::Ref<C78E::Texture2D> s_Texture;
		static uint32_t s_TextureId;
		static C78E::Renderer::FrameInfo s_TextureInfo;

		static uint32_t s_Width, s_Height;


	};

}
