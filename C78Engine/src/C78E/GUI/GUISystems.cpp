#include "C78ePCH.h"
#include "GUISystems.h"

namespace C78E::GUI {

	/*
	* DynamicElementSystem
	*/

	DynamicElementSystem::ToggleElementGroupButton::ToggleElementGroupButton(DynamicElementSystem* parent, const std::string& label, const std::vector<ElementIdentifier>& elementGroup)
		: TextButton(label,
			[this, parent]() {
				for (ElementIdentifier id : m_ElementGroup)
					parent->toggleShow(id);
			}
		), m_Label(label), m_ElementGroup(elementGroup) {

	}
	
	void DynamicElementSystem::show(Element& element) {
		if (shouldShow((ElementIdentifier)element)) {
			element.show();
		}
	}

	bool& DynamicElementSystem::shouldShow(ElementIdentifier element) {
		if (!m_ShowElement.contains(element))
			m_ShowElement[element] = true;
		return m_ShowElement[element];
	}

	void DynamicElementSystem::setShow(ElementIdentifier element) {
		m_ShowElement[element] = true;
	}

	void DynamicElementSystem::setHide(ElementIdentifier element) {
		m_ShowElement[element] = false;
	}

	void DynamicElementSystem::toggleShow(ElementIdentifier element) {
		if (!m_ShowElement.contains(element))
			m_ShowElement[element] = true;
		else
			m_ShowElement[element] = !m_ShowElement[element];
	}

	Ref<DynamicElementSystem::ToggleElementGroupButton> DynamicElementSystem::createToggleElementGroupButton(const std::string& label, const std::vector<ElementIdentifier>& elementGroup) {
		return createRef<DynamicElementSystem::ToggleElementGroupButton>(this, label, elementGroup);
	}


	/*
	* StringIndentationSystem
	*/
	StringIndentationSystem::StringIndentationSystem(uint32_t inititialIndent, const std::string& front, const std::string& back, const std::string& mid, const std::string& single)
		: m_Indent(inititialIndent), m_Front(front), m_Back(back), m_Mid(mid), m_Single(single) {
	}

	StringIndentationSystem::~StringIndentationSystem() { }

	void StringIndentationSystem::setIndent(uint32_t indent) { m_Indent = indent; }
	uint32_t StringIndentationSystem::getIndent() const { return m_Indent; }
	void StringIndentationSystem::pushIndent() { if (m_Indent + 1) m_Indent++; }
	void StringIndentationSystem::popIndent() { if (m_Indent) m_Indent--; }

	std::string StringIndentationSystem::getIndentationString() const {
		std::string out = "";
		if (m_Indent) {
			if (m_Indent == 1) {
				if (m_Single.empty()) {
					if (m_Front.empty()) {
						if (m_Back.empty()) {
							out += m_Mid;
						}
						else out += m_Back;
					}
					else out += m_Front;
				}
				else out += m_Single;
			}
			else {
				if (m_Front.empty()) out += m_Mid;
				else out += m_Front;
				for (uint32_t i = 0; i < m_Indent - 2; i++) out += m_Mid;
				if (m_Back.empty()) out += m_Mid;
				else out += m_Back;
			}
		}
		return out;
	}

	std::string StringIndentationSystem::getIndentationString(const std::string& suffix, const std::string& spacer) const {
		return getIndentationString() + spacer + suffix;
	}
}