#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class EditorMainMenuBar {
	public:

        static void init() {
            // load Language prefs
        }
        static void onUpdate() {}

		static void onImGuiRender() {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu(s_Label_File.c_str())) {
                    showFileSubmenu();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(s_Label_Edit.c_str())) {
                    showEditSubmenu();
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
		}
    private:
        static void showFileSubmenu() {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("impl!");

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As..")) {}
        }

        static void showEditSubmenu() {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        }

	private:
        static std::string s_Label_File;
        static std::string s_Label_Edit;
	};


    std::string EditorMainMenuBar::s_Label_File = "File";
    std::string EditorMainMenuBar::s_Label_Edit = "Edit";

}