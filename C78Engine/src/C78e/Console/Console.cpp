#include "C78ePCH.h"
#include "Console.h"

#include <C78e/Events/KeyEvent.h>


#include <imgui.h>
#include <imgui_internal.h>


namespace C78e {

	Console::Console(std::string consoleName) : Layer(consoleName), m_Title(consoleName) {
	}

	Console::~Console() {
	}

	// Portable helpers
	static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
	static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

	void Console::addLog(std::string logEntry) {
		m_Log.push_back(logEntry);
	}

	void Console::addCmd(std::string cmd, CmdCallbackFn callback) {
		m_Commands.emplace(cmd, callback);
	}

	void Console::onAttach()
	{
		addCmd("clear", BIND_CALLBACK_FN(Console::onCMDClear));
	}

	void Console::onDetach()
	{
		C78_CORE_INFO("Console stopped.")
	}

	void Console::onUpdate()
	{
	}

	void Console::onImGuiRender() {
		if (!m_Visible) return;

		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(m_Title.c_str(), &m_Visible)) {
			ImGui::End();
			return;
		}
		if (ImGui::BeginPopupContextItem()){
			if (ImGui::MenuItem("Close Console"))
				m_Visible = false;
			ImGui::EndPopup();
		}
		if (ImGui::SmallButton("Clear")) { clearLog(); }
		ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy");
		ImGui::Separator();

		if (ImGui::BeginPopup("Options")) {
			ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
			ImGui::EndPopup();
		}
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::Selectable("Clear")) clearLog();
				ImGui::EndPopup();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			if (copy_to_clipboard)
				ImGui::LogToClipboard();
			for (auto line : m_Log) {
				ImGui::TextUnformatted(line.c_str());
			}



			if (m_ScrollToBottom || (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
				ImGui::SetScrollHereY(1.0f);
			m_ScrollToBottom = false;
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
		ImGui::Separator();

		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", m_InputBuffer, IM_ARRAYSIZE(m_InputBuffer), input_text_flags, [](ImGuiInputTextCallbackData* data)->int{
			Console* console = (Console*)data->UserData;
			return console->commandTypeCallback(data);
		}, (void*)this))
		{
			char* s = m_InputBuffer;
			Strtrim(s);
			if (s[0])
				onExecute(s);
			strcpy(s, "");
			reclaim_focus = true;
		}
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}

	void Console::onExecute(const char* cmd) {
		addLog(cmd);
		m_History.push_back(cmd);
		for (auto pair : m_Commands) {
			if (pair.first == cmd) {
				(pair.second)(pair.first);
			}
		}


		m_ScrollToBottom = true;
	}

	int Console::commandTypeCallback(ImGuiInputTextCallbackData* data) {
		switch (data->EventFlag) {
			case ImGuiInputTextFlags_CallbackCompletion: {

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory: {

			}
		}
		return 0;
	}

	void Console::onEvent(Event& event) {
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_CALLBACK_FN(Console::onToggleConsole));

	}

	bool Console::onToggleConsole(KeyPressedEvent event) {
		if (event.GetKeyCode() == m_ToggleVisibleKey) {
			m_Visible = !m_Visible;
			return true;
		}
		return false;
		
	}
}