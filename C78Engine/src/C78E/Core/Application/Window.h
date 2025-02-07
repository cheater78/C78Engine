#pragma once

#include "C78E/Core/Core.h"
#include "C78E/Events/Event.h"

namespace C78E {

	// Interface representing a desktop system based Window
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		enum RefreshMode : int {
			Unlimited = 0,
			Vsync = 1,
			//Gsync / FreeSync
		};

		enum WindowMode {
			Windowed,
			FullScreen,
			BorderlessWindow
		};

		enum MouseMode {
			Normal = 0,
			Hidden = 1,
			Disabled = 2
		};

		struct Resolution {
			enum Type : uint8_t {
				nHD,
				VGA,
				SVGA,
				XGA,
				WXGA,
				WXGAe,
				SXGA,
				HD,
				HDe,
				WXGAp,
				HDp,
				UXGA,
				WSXGAp,
				FHD,
				WUXGA,
				QWXGA,
				QXGA,
				UWFHD,
				QHD,
				WQXGA,
				UWQHD,
				UHD_4K,
				UHD_5K,
				DUHD,
				UHD_8K
			};
			Resolution() : m_Resolution{ HD } {}
			Resolution(uint8_t type) : m_Resolution(type) {}
			Resolution(const Resolution& other) = default;
			~Resolution() = default;

			bool operator==(const Resolution& other) const { return this->m_Resolution == other.m_Resolution; }
			bool operator==(uint8_t other) const { return this->m_Resolution == other; }
			bool operator!=(const Resolution& other) const { return this->m_Resolution != other.m_Resolution; }
			bool operator!=(uint8_t other) const { return this->m_Resolution != other; }
			bool operator<(const Resolution& other) const { return this->m_Resolution < other.m_Resolution; }
			bool operator<(uint8_t other) const { return this->m_Resolution < other; }
			operator uint8_t() const { return m_Resolution; }

			static glm::ivec2 resolution(Resolution resolution);
			static std::string resolutionToString(const Resolution& resolution);
			static Resolution resolutionFromString(const std::string& resolutionString);

			static const std::array<glm::ivec2, 25>& resolutions();
			static const std::array<uint8_t, 25>& resolutionTypes();

		private:
			static const std::array<glm::ivec2, 25> s_Resolutions;
			uint8_t m_Resolution;
		};

		struct WindowProps {
			std::string title;
			uint32_t width;
			uint32_t height;
			WindowMode windowMode = WindowMode::Windowed;
			RefreshMode refreshMode = RefreshMode::Vsync;
			MouseMode mouseMode = MouseMode::Normal;
			EventCallbackFn eventCallback = nullptr;

			WindowProps(const std::string& title = "C78Engine Window",
				uint32_t width = 1920,
				uint32_t height = 1080,
				WindowMode windowMode = WindowMode::Windowed,
				RefreshMode refreshMode = RefreshMode::Vsync,
				MouseMode mouseMode = MouseMode::Normal)
				: title(title), width(width), height(height), windowMode(windowMode), refreshMode(refreshMode), mouseMode(mouseMode)
			{ }
		};
		typedef WindowProps WindowData;
	public:
		static Scope<Window> create(const WindowProps& props = WindowProps());
	public:
		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual void setResolution(const Resolution& resolution) = 0;

		virtual void* getNativeWindow() const = 0;
		virtual WindowProps getWindowProperties() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		
		virtual void setWindowMode(WindowMode windowMode) = 0;
		virtual WindowMode getWindowMode() const = 0;

		virtual void setRefreshMode(RefreshMode refreshMode) = 0;
		virtual RefreshMode getRefreshMode() const = 0;
		virtual bool isRefreshMode(RefreshMode refreshMode) const = 0;

		virtual MouseMode getMouseMode() const = 0;
		virtual void setMouseMode(MouseMode mouseMode) = 0;

		virtual std::string getClipBoardString() const = 0;
		virtual void setClipboardString(const std::string& str) = 0;
	};

}
