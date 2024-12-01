#include "C78EPCH.h"
#include "Console.h"

#include <C78E/Events/KeyEvent.h>

namespace C78E {

	Console::Console(std::string consoleName) 
		: Layer(consoleName), m_Title(consoleName)
	{ }

	Console::~Console() { }

	void Console::setPosSize(uint32_t posX, uint32_t posY, uint32_t sizeX, uint32_t sizeY) {
		m_ImGuiWinSizePos = { posX, posY, sizeX, sizeY };
	}

	void Console::resetPosSize(){
		Window& window = Application::get().getWindow();
		uint32_t winWidth = window.getWidth();
		uint32_t winHeight = window.getHeight();
		uint32_t border = std::min(winWidth, winHeight) / 32;
		setPosSize( 9 * winWidth / 32, border, 23 * winWidth / 32 - border, 18 * winHeight / 32);
	}

	void Console::addLog(std::string logEntry) {
		C78E_CORE_INFO(logEntry);
	}

	void Console::addCmd(std::string cmd, CmdCallbackFn callback) {
		m_Commands.emplace(cmd, callback);
	}

	void Console::onAttach() {
		addCmd("clear", BIND_CALLBACK_FN(Console::onCMDClear));
		addCmd("close", BIND_CALLBACK_FN(Console::onCMDClose));
	}

	void Console::onDetach() {
		C78E_CORE_INFO("Console stopped.")
	}

	void Console::onUpdate(Timestep delta) {
	}

	void Console::onImGuiRender() {
		if(!m_Visible) return;

		ImGui::SetNextWindowPos(ImVec2(static_cast<float>(m_ImGuiWinSizePos.posX), static_cast<float>(m_ImGuiWinSizePos.posY)), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(m_ImGuiWinSizePos.sizeX), static_cast<float>(m_ImGuiWinSizePos.sizeY)), ImGuiCond_Appearing);
		
		if (!ImGui::Begin(m_Title.c_str(), &m_Visible)) { ImGui::End(); return; }
		ImGui::SetWindowFocus();

		if (ImGui::BeginPopupContextItem()){
			if (ImGui::MenuItem("Close Console"))
				m_Visible = false;
			ImGui::EndPopup();
		}
		if (ImGui::SmallButton("Clear")) { clearLog(); }
		ImGui::Separator();

		if (ImGui::BeginPopup("Options")) {
			ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
			ImGui::EndPopup();
		}
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
			// TODO: Close
			if (false && ImGui::BeginPopupContextWindow()) {
				if (ImGui::Selectable("Clear")) clearLog();
				ImGui::EndPopup();
			}
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			for (auto& line : Log::getRingBufferSink()->last_formatted()) {
				ImGui::Text(line.c_str());
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
		if (
			ImGui::InputText("Command", m_InputBuffer, IM_ARRAYSIZE(m_InputBuffer), input_text_flags,
				[](ImGuiInputTextCallbackData* data)->int {
					Console* console = (Console*)data->UserData;
					return console->commandTypeCallback(data);
				},
				(void*)this)) {
			onEnter();
			reclaim_focus = true;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Enter")) { onEnter(); reclaim_focus = true; };
		

		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
		
		ImGui::End();
	}

	void Console::onEnter() {
		char* s = m_InputBuffer;
		if (s[0])
			onExecute(s);
		strcpy(s, "");
	}

	void Console::onExecute(const char* cmd) {
		addLog("> " + std::to_string(cmd));
		m_History.emplace_back(cmd);
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
				//TODO
			}
			case ImGuiInputTextFlags_CallbackHistory: {
				//TODO
			}
		}
		return 0;
	}

	bool Console::onEvent(Event& event) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			event.handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
		EventDispatcher dispatcher(event);
		if(dispatcher.dispatch<KeyPressedEvent>(BIND_CALLBACK_FN(Console::onToggleConsole))) return true;

		return false;
	}

	bool Console::onToggleConsole(KeyPressedEvent event) {
		if (event == m_ToggleVisibleKeyCombo) {
			m_Visible = !m_Visible;
			resetPosSize();
			return true;
		}
		return false;
		
	}
}