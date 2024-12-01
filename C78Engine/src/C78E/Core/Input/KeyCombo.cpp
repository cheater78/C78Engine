#include "C78EPCH.h"
#include "KeyCombo.h"

#include <C78E/Core/Input/Input.h>

namespace C78E::Input {

	void KeyCombo::pushKey(KeyCode keyCode) { m_KeyCombo.push_back(keyCode); }

	KeyCode KeyCombo::popKey() {
		KeyCode key = m_KeyCombo.back(); 
		m_KeyCombo.pop_back(); 
		return key; 
	}

	bool KeyCombo::eraseKey(KeyCode keyCode) {
		auto it = std::find(m_KeyCombo.begin(), m_KeyCombo.end(), keyCode);
		if(it == m_KeyCombo.end())
			return false;
		m_KeyCombo.erase(it);
		return true;
	}

	std::vector<KeyCode> KeyCombo::codes() const { return m_KeyCombo; }

	bool KeyCombo::matches(KeyPressedEvent e) const {
		const size_t size = m_KeyCombo.size();
		if(!size) return false;
		if(e.getKeyCode() != m_KeyCombo.back()) return false;
		for (size_t i = 0; i < size - 1; i++)
			if(!Input::isKeyPressed(m_KeyCombo[size - 1 - i]))
				return false;
		return true;
	}

	bool KeyCombo::matches(KeyEvent& e) const {
		if (e.getEventType() != EventType::KeyPressed)
			return false;
		return matches(static_cast<KeyPressedEvent&>(e));
	}
	
	bool KeyCombo::matches(Event& e) const {
		if (e.getEventType() != EventType::KeyPressed)
			return false;
		return matches(static_cast<KeyPressedEvent&>(e));
	}


	std::string KeyCombo::keyComboToString(const KeyCombo& keyCombo){
		std::string str;
		for(auto it = keyCombo.m_KeyCombo.begin(); it != keyCombo.m_KeyCombo.end(); ) {
			str += Key::keyCodeToString(*it);
			if(it++ != keyCombo.m_KeyCombo.end())
				str += " + ";
		}
		return str;
	}
	
	KeyCombo KeyCombo::keyComboFromString(const std::string& str) {
		KeyCombo combo;
		auto keyStrings = std::split(str, " + ");
		for(auto& keyString : keyStrings) {
			KeyCode key = Key::keyCodeFromString(keyString);
			if(key == Key::None) return KeyCombo{};
			combo.pushKey(key);
		}
		return combo;
	}

}

namespace std {

    _EXPORT_STD _NODISCARD string to_string(C78E::Input::KeyCombo keyCombo) {
		return C78E::Input::KeyCombo::keyComboToString(keyCombo);
	}

}