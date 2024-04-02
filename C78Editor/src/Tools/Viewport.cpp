#include "Viewport.h"

namespace C78Editor {

	C78E::Ref<C78E::Texture2D> Viewport::s_Texture = nullptr;
	uint32_t Viewport::s_TextureId = 0;

	uint32_t Viewport::s_Width = 1, Viewport::s_Height = 1;

	void Viewport::init()
	{
	}

	void Viewport::onUpdate(C78E::Ref<C78E::Texture2D> texture) {
		if (texture == nullptr) return;
		s_Texture = texture;
		s_TextureId = s_Texture->getRendererID();
	}

	void Viewport::onImGuiRender(bool& captureMouse) {
		
		ImGui::Begin("Viewport");
		if (s_Texture) {
			if (ImGui::IsKeyPressed(ImGuiKey_Escape))
				captureMouse = !captureMouse;

			ImVec2 size = ImGui::GetContentRegionAvail();
			uint32_t x = static_cast<uint32_t>(size.x);
			uint32_t y = static_cast<uint32_t>(size.y);

			if (x != s_Width || y != s_Height) {
				s_Width = x; s_Height = y;
				C78E::Renderer::onWindowResize(x, y);
			}

			ImGui::Image(
				(void*)(intptr_t)s_TextureId,
				ImVec2{ static_cast<float>(s_Texture->getWidth()), static_cast<float>(s_Texture->getHeight()) },
				ImVec2{ 0.f, 1.f },
				ImVec2{ 1.f, 0.f }
			);
		}
		ImGui::End();
	}

}
