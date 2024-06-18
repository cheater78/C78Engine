#pragma once
#include "GUIElements.h"
#include <C78E/Events/KeyEvent.h>
#include <C78E/GUI/HotkeyManager/KeyCombo.h>

namespace C78E::GUI {

	class MenuBarElement : public Element {
	public:
		virtual void show() override;

		virtual void onEvent(Event& event) = 0;
	protected:
		virtual void onContent() = 0;

	private:
	};

	class MenuBarItem : public Element {
	public:
		MenuBarItem() = delete;
		MenuBarItem(const std::string title = "Unnamed MenuBarItem", std::function<void(void)> onClick = nullptr, const std::string shortCutHint = " - ", GUI::KeyCombo shortCut = GUI::KeyCombo{});
		MenuBarItem(const MenuBarItem& other) = delete;
		~MenuBarItem() = default;

		virtual void show() override;

		virtual void onEvent(Event& event);
	private:
		std::string m_Title = "Unnamed MenuBarItem";
		std::string m_ShortCutHint = " - ";

	};

	class MenuBarMenu : public Element {
	public:
		MenuBarMenu() = default;
		MenuBarMenu(const MenuBarMenu& other) = delete;
		~MenuBarMenu() = default;

		virtual void show() override;

		virtual void onEvent(Event& event);
	private:
		std::string m_Title = "Unnamed MenuBarMenu";
		std::vector<MenuBarItem> m_Items;
	};


	class MenuBar {
	public:
		MenuBar() = default;
		MenuBar(const MenuBar& other) = delete;
		~MenuBar() = default;
	protected:
		virtual void show() final;
	public:
		virtual void onRender();

		virtual void onEvent(Event& event);
	private:
		std::vector<MenuBarMenu> m_Menus;
	};
}
