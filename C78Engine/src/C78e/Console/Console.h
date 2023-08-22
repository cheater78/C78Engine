#pragma once
#include "C78e/Core/Core.h"
#include "C78e/Core/Layer.h"
#include "C78e/Core/KeyCodes.h"

struct ImGuiInputTextCallbackData;

namespace C78e {

    class KeyPressedEvent;

    class Console : public Layer {
        using CmdCallbackFn = std::function<void(std::string)>;
    public:
        Console(std::string consoleName);
        ~Console();

        void addLog(std::string logEntry);

        void addCmd(std::string cmd, CmdCallbackFn callback);

        virtual void onAttach();
        virtual void onDetach();
        virtual void onUpdate(/* Timestep ts */);
        virtual void onImGuiRender();
        virtual void onEvent(Event& event);

        bool onToggleConsole(KeyPressedEvent event);

    private:
        std::string m_Title = "";
        bool m_Visible = true;
        //TODO: make variable
        KeyCode m_ToggleVisibleKey = Key::RightControl;

        std::unordered_map<std::string, const CmdCallbackFn> m_Commands{};
        std::vector<std::string> m_Log{};
        std::vector<std::string> m_History{};
        char m_InputBuffer[1024]{ 0 };
        bool m_AutoScroll = true;
        bool m_ScrollToBottom = false;

        bool m_UseImGui = true;
        bool m_BlockEvents = true;

        void onExecute(const char* cmd);
        void clearLog() { m_Log.clear(); }
        int commandTypeCallback(ImGuiInputTextCallbackData* data);



        void onCMDClear(std::string cmd) { this->clearLog(); }
        
    };

}
