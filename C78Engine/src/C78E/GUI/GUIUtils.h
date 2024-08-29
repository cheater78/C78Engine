#pragma once

#include <imgui.h>

namespace C78E::GUI {

	static ImVec2 toImVec(glm::vec2 v) { return ImVec2{ v.x, v.y }; };
	static ImVec4 toImVec(glm::vec4 v) { return ImVec4{ v.x, v.y, v.z, v.a }; };

	static glm::vec2 fromImVec(ImVec2 v) { return glm::vec2{ v.x, v.y }; };
	static glm::vec4 fromImVec(ImVec4 v) { return glm::vec4{ v.x, v.y, v.z, v.w }; };

	static void SameLine() { ImGui::SameLine(); }
	static const glm::vec2 autoSize() { return { 0.f, 0.f }; }
	static const glm::vec4 autoColor() { return { 0.f, 0.f, 0.f, 0.f }; }
	static const glm::vec4 markedColor() { return { .1f, .2f, .3f, 1.f }; }
	static const glm::vec4 noTintColor() { return { 1.f, 1.f, 1.f, 1.f }; }

	static glm::vec2 getLeftOverWindowSpace() { ImVec2 size = ImGui::GetContentRegionAvail(); return glm::vec2(size.x, size.y); }
	static glm::vec2 getFullWindowSpace() { ImVec2 size = ImGui::GetContentRegionMax(); return glm::vec2(size.x, size.y); }

}
