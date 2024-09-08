#include "Viewport.h"

namespace C78Editor {

	C78E::Ref<C78E::Texture2D> Viewport::s_Texture = nullptr;
	uint32_t Viewport::s_TextureId = 0;

	uint32_t Viewport::s_Width = 1, Viewport::s_Height = 1;

	C78E::Renderer::FrameInfo Viewport::s_TextureInfo{};

	void Viewport::init() { }

	void Viewport::onImGuiRender(bool& captureMouse, C78E::Ref<C78E::Renderer> renderer) {
		if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			captureMouse = !captureMouse;

		ImGui::Begin("Viewport");

		if (renderer) {
			s_Texture = renderer->getTargetTexture();
			s_TextureId = s_Texture->getRendererID();
			s_TextureInfo = renderer->getTargetInfo();

			ImVec2 size = ImGui::GetContentRegionAvail();
			uint32_t x = static_cast<uint32_t>(size.x);
			uint32_t y = static_cast<uint32_t>(size.y);

			if (x != s_Width || y != s_Height) {
				s_Width = x; s_Height = y;
				renderer->resizeTarget(s_Width, s_Height);
				s_Texture = nullptr;
				s_TextureId = 0;
			}

			if (s_Texture) {
				auto regionCorner = ImGui::GetCursorPos();
				ImGui::Image(
					(void*)(intptr_t)s_TextureId,
					ImVec2{ (float)x, (float)y },//static_cast<float>(s_Texture->getWidth()), static_cast<float>(s_Texture->getHeight())
					ImVec2{ 0.f, 1.f },
					ImVec2{ 1.f, 0.f }
				);

				ImGui::SetCursorPos(regionCorner);
				ImGui::Text(("Vertecies: " + std::to_string(s_TextureInfo.vertecies)).c_str());
				ImGui::Text(("Indicies: " + std::to_string(s_TextureInfo.indicies)).c_str());
				ImGui::Text(("DrawCalls: " + std::to_string(s_TextureInfo.drawCalls)).c_str());
			}
		}
		else {
			ImGui::Text("No Renderer!");
		}
		ImGui::End();
	}

}
