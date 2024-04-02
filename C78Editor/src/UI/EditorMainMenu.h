#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class EditorMainMenu {
	public:

		static void init() {
			s_Active = true;
		}

		static void onUpdate() {

		}

		static void onImGuiRender() {

		}

		static void setActive(bool active = true) { s_Active = active; }
		static bool isActive() { return s_Active; }

	private:
		static bool s_Active;

		//<project link>
	};
}