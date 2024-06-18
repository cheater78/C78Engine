#pragma once
#include "KeyCombo.h"

namespace C78E::GUI {
	/*
	class HotkeyManager {
	public:
		HotkeyManager() = default;
		HotkeyManager(const HotkeyManager& other) = delete;
		~HotkeyManager() = default;

		bool bind(const KeyCombo& keyBind, std::function<void(void)> exec, bool override = true);
		bool unbind(const KeyCombo& keyBind);

		bool onKeyPressedEvent(KeyPressedEvent event);
	private:
		bool isBound(const KeyCombo& keyBind);
	private:
		std::unordered_map<KeyCombo, std::function<void(void)>> m_ShortCuts;
	};
	*/
}