#pragma once
#include <C78E/Core/KeyCodes.h>
#include <C78E/Events/KeyEvent.h>

namespace C78E::GUI {

	/*
	* C78E::GUI::KeyCombo -> Representation of multiple concurrent KeyPresses
	* KeysPresses are stored in order, the last element in m_KeyCombo is the last key to be pressed in the KeyCombo,
	* which triggers the KeyPressEvent of this KeyCombo, all other Keys must be pressed at this time to match this KeyCombo
	* e.g.: m_KeyCombo[i] = { STRG, SHIFT, S }; // would be Strg + Shift + S
	*/
	class KeyCombo {
	public:
		KeyCombo(const std::vector<KeyCode>& keyCombo = {})
			:m_KeyCombo(keyCombo)
		{ }
		~KeyCombo() = default;

		void pushKey(KeyCode keyCode) { m_KeyCombo.push_back(keyCode); }
		KeyCode popKey() { KeyCode key = m_KeyCombo.back(); m_KeyCombo.pop_back(); return key; }

		bool matches(KeyPressedEvent e) const {
			bool match = true;
			const size_t size = m_KeyCombo.size();
			KeyPressedEvent& event = static_cast<KeyPressedEvent&>(e);
			for (size_t i = 0; i < size; i++) {
				const KeyCode& key = m_KeyCombo[size - 1 - i];
				if (!i) match &= (event.getKeyCode() == key);
				else match &= Input::isKeyPressed(key);
				if (!match) return false;
			}
			return true;
		}
		bool matches(KeyEvent& e) const {
			if (e.getEventType() != EventType::KeyPressed)
				return false;
			return matches(static_cast<KeyPressedEvent&>(e));
		}
		bool operator==(KeyPressedEvent& e) const {
			return matches(e);
		}
		/*
		bool operator==(KeyEvent& e) const {
			return matches(e);
		}
		bool operator==(Event& e) const {
			if ((e.getCategoryFlags() & EventCategory::EventCategoryInput) &&
				((e.getCategoryFlags() & EventCategory::EventCategoryKeyboard) || (e.getCategoryFlags() & EventCategory::EventCategoryMouseButton)))
				return matches(static_cast<KeyEvent&>(e));
			else return false;
		}
		*/
	private:
		std::vector<KeyCode> m_KeyCombo;

	};


	class KeyComboRecorder {
	public:
		//TODO
	private:
	};

}
