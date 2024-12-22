#pragma once

#include <imgui.h>

namespace C78E::GUI {

	static ImVec2 toImVec(glm::vec2 v) { return ImVec2{ v.x, v.y }; };
	static ImVec4 toImVec(glm::vec4 v) { return ImVec4{ v.x, v.y, v.z, v.a }; };

	static glm::vec2 fromImVec(ImVec2 v) { return glm::vec2{ v.x, v.y }; };
	static glm::vec4 fromImVec(ImVec4 v) { return glm::vec4{ v.x, v.y, v.z, v.w }; };

	static void SameLine(float relativeOffsetFromLeft = 0.f) {
		ImGui::SameLine(relativeOffsetFromLeft*ImGui::GetWindowContentRegionMax().x);
	}
	static const glm::vec2 autoSize() { return { 0.f, 0.f }; }
	static const glm::vec4 autoColor() { return { 0.f, 0.f, 0.f, 0.f }; }
	static const glm::vec4 markedColor() { return { .1f, .2f, .3f, 1.f }; }
	static const glm::vec4 noTintColor() { return { 1.f, 1.f, 1.f, 1.f }; }

	static glm::vec2 getLeftOverWindowSpace() { ImVec2 size = ImGui::GetContentRegionAvail(); return glm::vec2(size.x, size.y); }
	static glm::vec2 getFullWindowSpace() { ImVec2 size = ImGui::GetContentRegionMax(); return glm::vec2(size.x, size.y); }

	template<std::size_t bufferSize>
	static void drawLabeledTextInput(const std::string& label, std::string& input) {
		ImGui::PushID(label.c_str());
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		std::array<char, bufferSize> buffer = { 0 };
		std::memcpy(buffer.data(), input.data(), input.size());
		ImGui::InputText("", buffer.data(), bufferSize);
		input = std::string(buffer.data());
		ImGui::PopID();
	}

	static bool drawTextButton(const std::string& label) {
		ImGui::PushID(label.c_str());
		const bool pressed = ImGui::Button(label.c_str());
		ImGui::PopID();
		return pressed;
	}

	template<std::size_t bufferSize>
	static bool drawLabeledFileInput(const std::string& label, std::string& input, bool save, const std::vector<FileSystem::EntryType>& filter, const FilePath& filePickerBasePath = FileSystem::C78RootDirectory) {
		bool dialog = false;
		ImGui::PushID(label.c_str());
		drawLabeledTextInput<bufferSize>(label, input);
		ImGui::SameLine();
		if(drawTextButton("...")) {
			if(save)
				input = FileDialogs::saveFile(filter, filePickerBasePath, input, label).string();
			else
				input = FileDialogs::openFile(filter, filePickerBasePath, input, label).string();
			input = FileSystem::getRelativePathTo(input, filePickerBasePath).string();
			dialog = true;
		}
		ImGui::PopID();
		return dialog;
	}
	template<std::size_t bufferSize>
	static bool drawLabeledFileInput(const std::string& label, FilePath& input, bool save, const std::vector<FileSystem::EntryType>& filter, const FilePath& filePickerBasePath = FileSystem::C78RootDirectory) {
		std::string inputStr = input.string();
		bool dialog = drawLabeledFileInput<bufferSize>(label, inputStr, save, filter, filePickerBasePath);
		input = inputStr;
		return dialog;
	}

	template<typename T>
	static T drawLabeledComboInput(const std::string& label, T& value, const std::vector<T>& entries) {
		ImGui::PushID(label.c_str());
		ImGui::Text(label.c_str());
		ImGui::SameLine();

		std::vector<std::string> labelStrings;
		std::vector<const char*> labelStringsPtrs;
		labelStrings.reserve(entries.size());
		labelStringsPtrs.reserve(entries.size());
		int index = -1;
		for(auto entry : entries) {
			labelStrings.push_back(std::to_string(entry));
			labelStringsPtrs.push_back(labelStrings.back().c_str());
			if(value == entry)
				index = labelStringsPtrs.size() - 1;
		}

		ImGui::Combo("", &index, labelStringsPtrs.data(), labelStringsPtrs.size());

		ImGui::PopID();
		if(index > -1 && index < entries.size())
			value = entries.at(index);
		return value;
	}
	
}
