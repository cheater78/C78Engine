#pragma once

#include <C78E.h>
#include <C78Egui.h>
#include <C78Elibs.h>

namespace C78Editor::GUI {

	class ViewportUI {
	public:
		ViewportUI(C78E::Ref<C78E::ProjectManager> projectManager);
		~ViewportUI();

		void onEvent(C78E::Event& event);
		void onUpdate(C78E::Timestep dt);
		void onImGuiRender();

		void setActiveScene(C78E::Ref<C78E::Scene> scene);

	private:
		C78E::Ref<C78E::ProjectManager> m_ProjectManager;
		C78E::Ref<C78E::EditorCamera> m_EditorCamera;
		C78E::Ref<C78E::Scene> m_ActiveScene;
		C78E::Ref<C78E::Scene> m_EditorScene;
		C78E::Ref<C78E::Renderer> m_Renderer;

		float m_TargetScaling = 1.f;

		C78E::UUID m_ViewPortCamera = C78E::UUID::invalid();

	};

}
