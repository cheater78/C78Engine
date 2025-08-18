#pragma once

#include <C78E/Utils/StdUtils.h>
#include <C78E/Math/Math.h>

namespace C78E {

	enum WindowMode {
		Windowed,
		FullScreen,
		BorderlessWindow
	};

	enum WindowRefreshMode : int {
		Unlimited = 0,
		Vsync = 1,
		//TODO: Gsync / FreeSync
	};

	enum WindowMouseCursorMode {
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

		static ivec2 resolution(Resolution resolution);
		static std::string resolutionToString(const Resolution& resolution);
		static Resolution resolutionFromString(const std::string& resolutionString);

		static const std::array<glm::ivec2, 25>& resolutions();
		static const std::array<uint8_t, 25>& resolutionTypes();

	private:
		static const std::array<ivec2, 25> s_Resolutions;
		uint8_t m_Resolution;
	};

	struct WindowProperties {
		std::string title = "C78Engine Window";
		uvec2 size = Resolution::resolution(Resolution::FHD);
		WindowMode windowMode = WindowMode::Windowed;
		WindowRefreshMode refreshMode = WindowRefreshMode::Vsync;
		WindowMouseCursorMode mouseMode = WindowMouseCursorMode::Normal;
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::WindowMode& mode) {
		switch (mode) {
		case C78E::WindowMode::Windowed: return "Windowed";
		case C78E::WindowMode::FullScreen: return "FullScreen";
		case C78E::WindowMode::BorderlessWindow: return "BorderlessWindow";
		default: return "Unknown";
		}
	}

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::WindowRefreshMode& mode) {
		switch (mode) {
		case C78E::WindowRefreshMode::Unlimited: return "Unlimited";
		case C78E::WindowRefreshMode::Vsync: return "Vsync";
			//case C78E::WindowRefreshMode::Gsync: return "Gsync";
		default: return "Unknown";
		}
	}

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::WindowMouseCursorMode& mode) {
		switch (mode) {
		case C78E::WindowMouseCursorMode::Normal: return "Normal";
		case C78E::WindowMouseCursorMode::Hidden: return "Hidden";
		case C78E::WindowMouseCursorMode::Disabled: return "Disabled";
		default: return "Unknown";
		}
	}

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::Resolution& resolution) {
		return C78E::Resolution::resolutionToString(resolution);
	}

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::WindowProperties& props) {
		return "WindowProperties(title: " + props.title + 
			", size: " + to_string(props.size) +
			", windowMode: " + to_string(props.windowMode) +
			", refreshMode: " + to_string(props.refreshMode) +
			", mouseMode: " + to_string(props.mouseMode) + ")";
	}

}