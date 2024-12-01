#pragma once
#include <C78E.h>

namespace C78Editor {

	struct TypeControl {
	public:
		/*
		* Magic value columnWidth / <n+1>.6f
		* assigns the Labels size the ~golden ration of the 1/n part of width,
		* for n ElementControls
		*/

		static void drawFloat(const std::string& label, float& value, float resetValue = 0.0f, bool drawColorStyle = false, float width = 100.0f) {
			ImFont* boldFont = nullptr;
			ImVec2 buttonSize{};
			openConrolBlock(boldFont, buttonSize, label, width / 2.6f);
			drawXR(boldFont, drawColorStyle, buttonSize, value, resetValue);
			closeControlBlock();
		}
		static void drawFloat2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, bool drawColorStyle = false, float width = 100.0f) {
			ImFont* boldFont = nullptr;
			ImVec2 buttonSize{};
			openConrolBlock(boldFont, buttonSize, label, width / 3.6f);
			drawXR(boldFont, drawColorStyle, buttonSize, *(float*)&values.x, resetValue);
			ImGui::SameLine();
			drawYG(boldFont, drawColorStyle, buttonSize, *(float*)&values.y, resetValue);
			closeControlBlock();
		}
		static void drawFloat3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, bool drawColorStyle = false, float width = 100.0f) {
			ImFont* boldFont = nullptr;
			ImVec2 buttonSize{};
			openConrolBlock(boldFont, buttonSize, label, width / 4.6f);
			drawXR(boldFont, drawColorStyle, buttonSize, *(float*)&values.x, resetValue);
			ImGui::SameLine();
			drawYG(boldFont, drawColorStyle, buttonSize, *(float*)&values.y, resetValue);
			ImGui::SameLine();
			drawZB(boldFont, drawColorStyle, buttonSize, *(float*)&values.z, resetValue);
			closeControlBlock();
		}
		static void drawFloat4(const std::string& label, glm::vec4& values, float resetValue = 0.0f, bool drawColorStyle = false, float width = 100.0f) {
			ImFont* boldFont = nullptr;
			ImVec2 buttonSize{};
			openConrolBlock(boldFont, buttonSize, label, width / 5.6f);
			drawXR(boldFont, drawColorStyle, buttonSize, *(float*)&values.x, resetValue);
			ImGui::SameLine();
			drawYG(boldFont, drawColorStyle, buttonSize, *(float*)&values.y, resetValue);
			ImGui::SameLine();
			drawZB(boldFont, drawColorStyle, buttonSize, *(float*)&values.z, resetValue);
			ImGui::SameLine();
			drawWA(boldFont, drawColorStyle, buttonSize, *(float*)&values.w, resetValue);
			closeControlBlock();
		}
	private:
		static void openConrolBlock(ImFont* boldFont, ImVec2& buttonSize, const std::string& label, float columnWidth) {
			ImGuiIO& io = ImGui::GetIO();
			boldFont = io.Fonts->Fonts[0];

			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			buttonSize = { lineHeight + 3.0f, lineHeight };
		}
		static void closeControlBlock() {
			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}
		static void drawXR(ImFont* boldFont, bool drawColorStyle, ImVec2 buttonSize, float& value, float resetValue) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button((drawColorStyle) ? "R" : "X", buttonSize))
				value = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		static void drawYG(ImFont* boldFont, bool drawColorStyle, ImVec2 buttonSize, float& value, float resetValue) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button((drawColorStyle) ? "G" : "Y", buttonSize))
				value = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		static void drawZB(ImFont* boldFont, bool drawColorStyle, ImVec2 buttonSize, float& value, float resetValue) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button((drawColorStyle) ? "B" : "Z", buttonSize))
				value = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		static void drawWA(ImFont* boldFont, bool drawColorStyle, ImVec2 buttonSize, float& value, float resetValue) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button((drawColorStyle) ? "A" : "W", buttonSize))
				value = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##W", &value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
	};

}
