#pragma once
#include <C78E/Events/KeyEvent.h>

namespace C78E::Input {

	/*
	* C78E::Input::KeyCombo -> Representation of multiple concurrent KeyPresses
	* KeysPresses are stored in order, the last element in m_KeyCombo is the last key to be pressed in the KeyCombo,
	* which triggers the KeyPressEvent of this KeyCombo, all other Keys must be pressed at this time to match this KeyCombo
	* e.g.: KeyCombo{ STRG, SHIFT, S }; // would be Strg + Shift + S, with Key::S triggering the match
	*/
	class KeyCombo {
	public:
		KeyCombo(const std::vector<KeyCode>& keyCombo = {})
			: m_KeyCombo(keyCombo)
		{ }
		~KeyCombo() = default;

		void pushKey(KeyCode keyCode);
		KeyCode popKey();
		bool eraseKey(KeyCode keyCode);

		std::vector<KeyCode> codes() const;

		bool matches(KeyPressedEvent e) const;
		bool matches(KeyEvent& e) const;
		bool matches(Event& e) const;

		bool operator==(KeyPressedEvent& e) const { return matches(e); }
		bool operator==(KeyEvent& e) const { return matches(e); }
		bool operator==(Event& e) const { return matches(e); }

	public:
		static std::string keyComboToString(const KeyCombo& keyCombo);
		static KeyCombo keyComboFromString(const std::string& str);
	private:
		std::vector<KeyCode> m_KeyCombo;
	};


	class KeyComboRecorder {
	public:
		KeyComboRecorder() = default;
		KeyComboRecorder(const KeyComboRecorder& other) = delete;
		~KeyComboRecorder() = default;

		void start() {
			m_Combo = KeyCombo();
			m_Recording = true;
		}

		void reset() {
			m_Combo = KeyCombo();
			m_Recording = false;
		}

		bool onEvent(Event& e) {
			if (e.getCategoryFlags() & EventCategory::EventCategoryInput)
				if(e.getEventType() == EventType::KeyPressed) {
					KeyPressedEvent event = static_cast<KeyPressedEvent&>(e);
					m_Combo.pushKey(event.getKeyCode());
					return true;
				} else if(e.getEventType() == EventType::KeyReleased) {
					KeyReleasedEvent event = static_cast<KeyReleasedEvent&>(e);
					if(m_Combo.codes().back() == event.getKeyCode())
						m_Recording = false;
					else
						return m_Combo.eraseKey(event.getKeyCode());
				} 
			return false;
		}

		bool hasFinished() const { return m_Recording && m_Combo.codes().size(); }
		KeyCombo getKeyCombo() const { return m_Combo; }
		
	private:
		KeyCombo m_Combo;
		bool m_Recording = false;
	};

}

namespace std {
	_EXPORT_STD _NODISCARD string to_string(C78E::Input::KeyCombo keyCombo);
}