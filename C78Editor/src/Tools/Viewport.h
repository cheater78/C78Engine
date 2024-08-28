#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class Viewport {
	public:

		static void init();
		static void onUpdate(C78E::Ref<C78E::Texture2D> texture);
		static void onImGuiRender(bool& captureMouse, C78E::Ref<C78E::Renderer> renderer);

	private:
		static C78E::Ref<C78E::Texture2D> s_Texture;
		static uint32_t s_TextureId;

		static uint32_t s_Width, s_Height;


	};

}
