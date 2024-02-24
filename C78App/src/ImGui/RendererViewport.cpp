#include "RendererViewport.h"

#include "imgui/imgui.h"

namespace C78Editor {

	C78E::Ref<C78E::Texture2D> RendererViewport::s_Texture = nullptr;
	uint32_t RendererViewport::s_TextureId = 0;

	uint32_t RendererViewport::s_Width = 1, RendererViewport::s_Height = 1;

	void RendererViewport::init()
	{
	}

	void RendererViewport::onUpdate(C78E::Ref<C78E::Texture2D> texture) {
		s_Texture = texture;
		s_TextureId = s_Texture->getRendererID();
	}

	void RendererViewport::onImGuiRender() {
		if (s_Texture == nullptr) return;
		ImGui::Begin("Viewport");

		ImVec2 size = ImGui::GetContentRegionAvail();
		uint32_t x = static_cast<uint32_t>(size.x);
		uint32_t y = static_cast<uint32_t>(size.y);

		if (x != s_Width || y != s_Height) {
			s_Width = x; s_Height = y;
			C78E::Renderer::onWindowResize(x, y);
		}

		ImGui::Image(
			(void*)(intptr_t)s_TextureId,
			ImVec2{static_cast<float>(s_Texture->getWidth()), static_cast<float>(s_Texture->getHeight())},
			ImVec2{0.f, 1.f},
			ImVec2{1.f, 0.f}
		);
		ImGui::End();
	}

}
