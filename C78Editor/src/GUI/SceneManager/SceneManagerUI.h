#pragma once

#include <C78E.h>
#include <C78Egui.h>
#include <C78Elibs.h>

namespace C78Editor::GUI {

	class SceneManagerUI {
	public:
		SceneManagerUI(C78E::Ref<C78E::SceneManager> sceneManager);
		SceneManagerUI(const SceneManagerUI& other) = delete;
		~SceneManagerUI();

		void onImGuiRender();


	private:
		C78E::WRef<C78E::SceneManager> m_SceneManager;

	};

}
