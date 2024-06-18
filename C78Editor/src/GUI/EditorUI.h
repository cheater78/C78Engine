#pragma once

#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class EditorUI {
	public:
		EditorUI(C78E::Ref<C78E::ProjectManager> projectManager, C78E::Ref<C78E::SceneManager> sceneManager);
		~EditorUI();

		void onImGuiMainMenuBar();
		bool onImGuiRender();

	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;
		C78E::Ref<C78E::SceneManager> m_SceneManager;
	};

}
