#pragma once
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/Log.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Core/Input/KeyCodes.h>

struct ImGuiInputTextCallbackData;

namespace C78E {

    class KeyPressedEvent;

    namespace Input {
        class KeyCombo;
	}

    class Console {
        using CmdCallbackFn = std::function<void(std::string)>;
    public:
        Console(Window& window, std::string consoleName);
        ~Console();

        void setPosSize(uint32_t posX, uint32_t posY, uint32_t sizeX, uint32_t sizeY);
        void resetPosSize();

        void addLog(std::string logEntry);

        void addCmd(std::string cmd, CmdCallbackFn callback);

        virtual void onDebugRender();
        virtual void onEvent(Event& event);

        void show(bool show) { m_Visible = show; }
        void onToggleConsole(KeyPressedEvent event);

    private:
        Window& m_Window;
        std::string m_Title = "";
        bool m_Visible = false;

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
