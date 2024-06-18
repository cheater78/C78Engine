#include "C78ePCH.h"
#include "HotkeyManager.h"
#include "KeyCombo.h"

namespace C78E::GUI {

	/*
	bool HotkeyManager::bind(const KeyCombo& keyBind, std::function<void(void)> exec, bool override) {
		if ( (isBound(keyBind) && !override) || !exec ) return false;
		m_ShortCuts[keyBind] = exec;
		return true;
	}

	bool HotkeyManager::unbind(const KeyCombo& keyBind) {
		if (!isBound(keyBind)) return false;
		m_ShortCuts.erase(keyBind);
		return true;
	}

	bool HotkeyManager::onKeyPressedEvent(KeyPressedEvent event) {
		for (auto& kv : m_ShortCuts) {
			if (kv.first.matches(event)) {
				kv.second();
				return true;
			}
		}
		return false;
	}

	bool HotkeyManager::isBound(const KeyCombo& keyBind) {
		return m_ShortCuts.contains(keyBind);
	}
	*/
}
