#include "C78ePCH.h"
#include "ImGuiUtils.h"

namespace C78E {

	/*
	* Element
	*/
	std::random_device Gui::Element::s_RandomDevice;
	std::mt19937_64 Gui::Element::s_Engine(Gui::Element::s_RandomDevice());
	std::uniform_int_distribution<int> Gui::Element::s_UniformDistribution;

	Gui::Element::Element()
		: m_Handle(s_UniformDistribution(s_Engine))
	{ }

	Gui::Element::~Element() { }

	void Gui::Element::begin() {
		ImGui::PushID(m_Handle);
	}

	void Gui::Element::end() {
		ImGui::PopID();
	}



	/*
	* TextButton
	*/
	Gui::TextButton::TextButton(std::string label, std::function<void(void)> onClick) 
		: m_Label(label), m_OnClick(onClick) {
	}

	Gui::TextButton::~TextButton() { }

	void Gui::TextButton::show() {
		begin();
		if (ImGui::Button(m_Label.c_str()))
			m_OnClick();
		end();
	}


	/*
	* CyclingTextButton
	*/
	Gui::CyclingTextButton::CyclingTextButton(std::vector<std::string> labels, std::vector<std::function<void(void)>> onClicks, uint32_t initState)
		: m_Label(labels), m_OnClicks(onClicks), m_State(initState) {
		C78_CORE_ASSERT(m_Label.size() && m_OnClicks.size(), "Gui::CyclingTextButton::CyclingTextButton: neither Labels nor onClick functions can be empty!");
		C78_CORE_ASSERT(m_Label.size() == m_OnClicks.size(), "Gui::CyclingTextButton::CyclingTextButton: every Label must have an associated onClick function!");
	}

	Gui::CyclingTextButton::~CyclingTextButton() { }

	void Gui::CyclingTextButton::show() {
		begin();
		if (ImGui::Button(m_Label.at(m_State).c_str())) {
			m_OnClicks.at(m_State)();

			m_State = (m_State + 1) % m_Label.size();
		}
		end();
	}


	/*
	* TextInput
	*/
	Gui::TextInput::TextInput(std::string label, std::string defaultValue, size_t maxLength, Ref<Buffer> buffer)
		: m_Label(label) {
		if (!buffer) {
			m_Buffer = createRef<Buffer>(maxLength);
		}
		else {
			m_Buffer = buffer;
		}
		setContent(defaultValue);
	}

	Gui::TextInput::~TextInput() { /* dropping the last Ref to m_Buffer will free its memory */ }

	void Gui::TextInput::setContent(const std::string& content) {
		C78_CORE_ASSERT(content.size() <= m_Buffer->size, "Gui::TextInput::setContent: new Content can't be larger than Buffer size!");
		m_Buffer->clear<char>(0);
		if (!content.size()) return;
		memcpy_s(m_Buffer->data, content.size(), content.c_str(), content.size());
	}

	std::string Gui::TextInput::getContent() {
		std::string out = "";
		for (size_t off = 0; off < m_Buffer->size; off++) {
			const char* chr = ((char*)m_Buffer->data + off);
			if (*chr < ' ' || *chr > '~') break;
			else out += *chr;
		}
		return out;
	}

	void Gui::TextInput::show() {
		begin();
		ImGui::Text(m_Label.c_str());
		ImGui::SameLine();
		ImGui::InputText("", (char*)m_Buffer->data, m_Buffer->size);
		end();
	}

}

