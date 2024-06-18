#pragma once

#include <C78E.h>
#include <C78Egui.h>
#include <C78Elibs.h>

namespace C78Editor::GUI {

	class ProjectManager;

	

	class CreateProjectUI : public C78E::GUI::Element {
	public:
		CreateProjectUI(::C78Editor::GUI::ProjectManager* projectManager);
		~CreateProjectUI();

		virtual void show() override final;

		virtual void widget();
	private:
		::C78Editor::GUI::ProjectManager* m_ProjectManager;

		C78E::Ref<C78E::GUI::FileManager> m_ProjectFileManager;
		C78E::Ref<C78E::GUI::FileManager> m_AssetFileManager;
		C78E::Ref<C78E::GUI::TextButton> m_CancelTB;
		C78E::Ref<C78E::GUI::TextButton> m_CreateProjectTB;

	};

	class OpenProjectUI : public C78E::GUI::Element {
	public:
		OpenProjectUI(::C78Editor::GUI::ProjectManager* projectManager);
		~OpenProjectUI();

		virtual void show() override final;

		virtual void widget();
	private:
		::C78Editor::GUI::ProjectManager* m_ProjectManager;
		C78E::Ref<C78E::GUI::FileManager> m_FileManager;
	};


	class ProjectManagerUI {
	public:
		ProjectManagerUI(::C78Editor::GUI::ProjectManager* projectManager);
		~ProjectManagerUI();

		virtual void onImGuiRender();

	private:
		void onWelcomeScreen(); //TODO: move to own UIWindow
	private:
		enum UIState {
			WELCOME,
			CREATEPROJECT,
			OPENPROJECT,
			VIEWPROJECT
		};
	private:
		UIState m_UIState = UIState::WELCOME;

		::C78Editor::GUI::ProjectManager* m_ProjectManager;

		CreateProjectUI m_CreateProjectUI;
		OpenProjectUI m_OpenProjectUI;
	};

}
