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

        void setPosSize(uint32_t posX, uint32_t posY, uint32_t sizeX, uint32_t sizeY);

        void addLog(std::string logEntry);

        void addCmd(std::string cmd, CmdCallbackFn callback);

        virtual void onAttach();
        virtual void onDetach();
        virtual void onUpdate(Timestep ts);
        virtual void onImGuiRender();
        virtual void onEvent(Event& event);

        void show(bool show) { m_Visible = show; }
        bool onToggleConsole(KeyPressedEvent event);

    private:
        std::string m_Title = "";
        bool m_Visible = true;
        //TODO: make variable
        KeyCode m_ToggleVisibleKey = Key::RightControl;

        std::unordered_map<std::string, const CmdCallbackFn> m_Commands{};
        std::vector<std::string> m_History{};
        char m_InputBuffer[1024]{ 0 };
        bool m_AutoScroll = true;
        bool m_ScrollToBottom = false;

        bool m_UseImGui = true;
        bool m_BlockEvents = true;
        struct WinSizePos {
            uint32_t posX;
            uint32_t posY;
            uint32_t sizeX;
            uint32_t sizeY;
        } m_ImGuiWinSizePos;

        void onEnter();
        void onExecute(const char* cmd);
        void clearLog() { Log::getCoreLogger()->flush(); Log::getClientLogger()->flush(); }
        int commandTypeCallback(ImGuiInputTextCallbackData* data);



        void onCMDClear(std::string cmd) { this->clearLog(); }
        void onCMDClose(std::string cmd) { this->m_Visible = false; }
        
    };

}
