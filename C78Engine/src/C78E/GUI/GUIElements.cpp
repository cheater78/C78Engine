#include "C78ePCH.h"
#include "GUIElements.h"

#include "GUIUtils.h"
namespace C78E::GUI {
	
	/*
	* Visible
	*/
	Visible::Visible(bool visible) 
		: m_Visible(visible)
	{ }

	void Visible::setVisible(bool visible) {
		m_Visible = visible;
	}

	bool Visible::isVisible() const {
		return m_Visible;
	}
	
	/*
	* UIContext
	*/
	UIContext::UIContext() {
		getSpecs();
	}

	const UIContext::Specs UIContext::getSpecs() {
		m_Specs.monitor = C78E::System::getPrimaryMonitor(); //TODO: getWindow -> determine Monitor (not always primary)

		return m_Specs;
	}

	glm::vec2 UIContext::toAbsolute(glm::vec2 relative) const { return glm::vec2(relative.x * m_Specs.monitor.width, relative.y * m_Specs.monitor.height); }
	glm::vec2 UIContext::toRelative(glm::vec2 absolute) const { return glm::vec2(absolute.x / m_Specs.monitor.width, absolute.y / m_Specs.monitor.height); }
	
	glm::vec2 UIContext::getScreenSize() const { return glm::vec2(m_Specs.monitor.width, m_Specs.monitor.height); }
	float UIContext::getShorterScreenSideLength() const { return std::min<float>(m_Specs.monitor.width, m_Specs.monitor.height); }
	float UIContext::getLongerScreenSideLength() const { return std::max<float>(m_Specs.monitor.width, m_Specs.monitor.height); }

	glm::vec2 UIContext::resizeToScale(glm::vec2 size) const { return m_Scale * size; }


	/*
	* Element
	*/
	std::random_device Element::s_RandomDevice;
	std::mt19937_64 Element::s_Engine(Element::s_RandomDevice());
	std::uniform_int_distribution<int> Element::s_UniformDistribution;

	Element::Element()
		: Visible(), UIContext(), m_Handle(s_UniformDistribution(s_Engine))
	{ }

	Element::~Element() { }

	Element::Style& Element::style() { return m_Style; }

	void Element::begin() {
		ImGui::PushID(m_Handle);
		m_Style.pushStyle();
	}

	void Element::end() {
		m_Style.popStyle();
		ImGui::PopID();
	}

	/*
	* Element::Style
	*/
	void Element::Style::pushStyle() {
		glm::vec2 parentSpace = GUI::getFullWindowSpace();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, disabledAlpha);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, toImVec(itemSpacing));

		ImGui::PushItemWidth(parentRelativeItemWidth * parentSpace.x);
	}

	void Element::Style::popStyle() {
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	

	/*
	* Window
	*/
	Window::Window(const std::string& title, const Style& style)
		: Element() {
		if (!title.empty()) m_Title = title;
	}
	void Window::show() {
		if (!isVisible()) return;
		ImGui::Begin(m_Title.c_str(), &m_Visible, m_Style.windowFlags);
		begin();
		content();
		end();
		ImGui::End();
	}

	Window::Style& Window::style() { return m_Style; }

	void Window::setTitle(const std::string& title) { m_Title = title; }
	std::string Window::getTitle(const std::string& title) { return m_Title; }

	/*
	* Window::Style
	*/
	void Window::Style::pushStyle() {
		Element::Style::pushStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, toImVec(windowPadding));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, windowBorderSize);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, toImVec(windowMinSize));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, toImVec(windowTitleAlign));
	}

	void Window::Style::popStyle() {
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		Element::Style::popStyle();
	}

	/*
	* Text
	*/
	Text::Text(const std::string& label)
		: m_Label(label) {
	}

	Text::~Text() { }

	void Text::show() {
		if (!m_Visible) return;
		begin();
		ImGui::Text(m_Label.c_str());
		end();
	}


	/*
	* TextButton
	*/
	TextButton::TextButton(std::string label, std::function<void(void)> onClick)
		: m_Label(label), m_OnClick(onClick) {
	}

	TextButton::~TextButton() { }

	void TextButton::show() {
		if (!m_Visible) return;
		begin();
		if (ImGui::Button(m_Label.c_str()))
			m_OnClick();
		end();
	}


	/*
	* CyclingTextButton
	*/
	CyclingTextButton::CyclingTextButton(std::vector<std::string> labels, std::vector<std::function<void(void)>> onClicks, uint32_t initState)
		: m_Label(labels), m_OnClicks(onClicks), m_State(initState) {
		C78_CORE_ASSERT(m_Label.size() && m_OnClicks.size(), "Gui::CyclingTextButton::CyclingTextButton: neither Labels nor onClick functions can be empty!");
		C78_CORE_ASSERT(m_Label.size() == m_OnClicks.size(), "Gui::CyclingTextButton::CyclingTextButton: every Label must have an associated onClick function!");
	}

	CyclingTextButton::~CyclingTextButton() { }

	void CyclingTextButton::show() {
		if (!m_Visible) return;
		begin();
		if (ImGui::Button(m_Label.at(m_State).c_str())) {
			m_OnClicks.at(m_State)();

			m_State = (m_State + 1) % m_Label.size();
		}
		end();
	}

	/*
	* ImageButton
	*/
	ImageButton::ImageButton(std::string label, LabelPostition labelPos, std::string toolTip, TextureHandle texture, std::function<void(void)> onClick)
		: m_Label(label), m_LabelPostition(labelPos), m_Tooltip(toolTip), m_Texture(texture), m_OnClick(onClick) { }

	ImageButton::~ImageButton() { }

	void ImageButton::show() {
		if (!m_Visible) return;
		begin();
		if (m_LabelPostition == ABOVE || m_LabelPostition == LEFT) ImGui::TextWrapped(m_Label.c_str());
		if (m_LabelPostition == LEFT) ImGui::SameLine();

		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		C78_CORE_WARN("TODO: ImGui Element Styling!");
		if (
			ImGui::ImageButton(
				m_Texture,
				{ 10.f, 10.f },
				{ 0,0 },
				{ 1,1 },
				0//,
				//{ backGroundColor.r, backGroundColor.g, backGroundColor.b, backGroundColor.a },
				//{ tintColor.r, tintColor.g, tintColor.b, tintColor.a }
			)
			)
			m_OnClick();

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			if (!m_Tooltip.empty())
				ImGui::SetTooltip(m_Tooltip.c_str());
		
		//ImGui::PopStyleVar();

		if (m_LabelPostition == RIGHT) ImGui::SameLine();
		if (m_LabelPostition == BELOW || m_LabelPostition == RIGHT) ImGui::TextWrapped(m_Label.c_str());
		
		end();
	}

	/*
	* TextInput
	*/
	TextInput::TextInput(const std::string& label, const std::string& defaultValue, size_t maxLength, Ref<Buffer> buffer)
		: m_Label(label) {
		if (!buffer) {
			m_Buffer = createRef<Buffer>(maxLength);
		}
		else {
			m_Buffer = buffer;
		}
		setContent(defaultValue);
	}

	TextInput::~TextInput() { /* dropping the last Ref to m_Buffer will free its memory */ }

	void TextInput::setContent(const std::string& content) {
		C78_CORE_ASSERT(content.size() <= m_Buffer->size, "Gui::TextInput::setContent: new Content can't be larger than Buffer size!");
		m_Buffer->clear<char>(0);
		if (!content.size()) return;
		memcpy_s(m_Buffer->data, content.size(), content.c_str(), content.size());
	}

	std::string TextInput::getContent() {
		std::string out = "";
		for (size_t off = 0; off < m_Buffer->size; off++) {
			const char* chr = ((char*)m_Buffer->data + off);
			if (*chr < ' ' || *chr > '~') break;
			else out += *chr;
		}
		return out;
	}

	void TextInput::show() {
		if (!m_Visible) return;
		begin();
		ImGui::Text(m_Label.c_str());
		ImGui::SameLine();
		//if (width) ImGui::PushItemWidth(width - ImGui::CalcTextSize(m_Label.c_str()).x);
		ImGui::InputText("", (char*)m_Buffer->data, m_Buffer->size);
		//if (width) ImGui::PopItemWidth();
		end();
	}


	/*
	* DragFloat
	*/
	DragFloat::DragFloat(float* values, const std::string& label, float step, float min, float max, const std::string& format)
		: m_Step(step), m_Min(min), m_Max(max), m_Value(values), m_Label(label), m_Format(format) {
		if (!m_Value)
			m_Value = (float*)malloc(sizeof(float));
	}

	DragFloat::~DragFloat() {
		if (m_Value)
			free(m_Value);
	}

	void DragFloat::show() {
		if (!m_Visible) return;
		begin();
		ImGui::DragFloat(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max, m_Format.c_str());
		end();
	}

	float& DragFloat::getValue() {
		return *m_Value;
	}

	void DragFloat::setStep(float step) { m_Step = step; }
	void DragFloat::setMin(float min) { m_Min = min; }
	void DragFloat::setMax(float max) { m_Max = max; }



	/*
	* DragFloat2
	*/
	DragFloat2::DragFloat2(float* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat(values, label, step, min, max, format) { // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(2 * sizeof(float)); // remalloc 2 floats if not values
		}
	}

	DragFloat2::DragFloat2(glm::vec2* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat((float*)values, label, step, min, max, format) {  // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(2 * sizeof(float)); // remalloc 2 floats if not values
		}
	}

	DragFloat2::~DragFloat2() {
	}

	void DragFloat2::show() {
		if (!m_Visible) return;
		begin();
		ImGui::DragFloat2(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max, m_Format.c_str());
		end();
	}

	glm::vec2& DragFloat2::getValue() {
		return *(glm::vec2*)m_Value;
	}


	/*
	* DragFloat3
	*/
	DragFloat3::DragFloat3(float* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat(values, label, step, min, max, format) { // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(3 * sizeof(float)); // remalloc 3 floats if not values
		}
	}

	DragFloat3::DragFloat3(glm::vec3* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat((float*)values, label, step, min, max, format) { // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(3 * sizeof(float)); // remalloc 3 floats if not values
		}
	}

	DragFloat3::~DragFloat3() {
	}

	void DragFloat3::show() {
		if (!m_Visible) return;
		begin();
		ImGui::DragFloat3(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max, m_Format.c_str());
		end();
	}

	glm::vec3& DragFloat3::getValue() {
		return *(glm::vec3*)m_Value;
	}


	/*
	* DragFloat4
	*/
	DragFloat4::DragFloat4(float* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat(values, label, step, min, max, format) { // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(4 * sizeof(float)); // remalloc 4 floats if not values
		}
	}

	DragFloat4::DragFloat4(glm::vec4* values, const std::string& label, float step, float min, float max, const std::string& format)
		: DragFloat((float*)values, label, step, min, max, format) { // mallocs 1 float if not values
		if (m_Value && !values) {
			free(m_Value);
			m_Value = (float*)malloc(4 * sizeof(float)); // remalloc 4 floats if not values
		}
	}

	DragFloat4::~DragFloat4() {
	}

	void DragFloat4::show() {
		if (!m_Visible) return;
		begin();
		ImGui::DragFloat4(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max, m_Format.c_str());
		end();
	}

	glm::vec4& DragFloat4::getValue() {
		return *(glm::vec4*)m_Value;
	}

}