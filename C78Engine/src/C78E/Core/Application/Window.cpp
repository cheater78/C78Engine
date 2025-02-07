#include "C78EPCH.h"
#include "C78E/Core/Application/Window.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"

namespace C78E {

	const std::array<glm::ivec2, 25> Window::Resolution::s_Resolutions = {
		glm::ivec2{ 640,  360},
		glm::ivec2{ 640,  480},
		glm::ivec2{ 800,  600},
		glm::ivec2{1024,  768},
		glm::ivec2{1280,  720},
		glm::ivec2{1280,  800},
		glm::ivec2{1280, 1024},
		glm::ivec2{1360,  768},
		glm::ivec2{1366,  768},
		glm::ivec2{1440,  900},
		glm::ivec2{1600,  900},
		glm::ivec2{1600, 1200},
		glm::ivec2{1680, 1050},
		glm::ivec2{1920, 1080},
		glm::ivec2{1920, 1200},
		glm::ivec2{2048, 1152},
		glm::ivec2{2048, 1536},
		glm::ivec2{2560, 1080},
		glm::ivec2{2560, 1440},
		glm::ivec2{2560, 1600},
		glm::ivec2{3440, 1440},
		glm::ivec2{3840, 2160},
		glm::ivec2{5120, 2880},
		glm::ivec2{7680, 2160},
		glm::ivec2{7680, 4320}
	};
	
	Scope<Window> Window::create(const WindowProps& props) {
	#ifdef C78E_PLATFORM_WINDOWS
		return createScope<WindowsWindow>(props);
	#elif defined C78E_PLATFORM_LINUX
		return createScope<LinuxWindow>(props);
	#else
		C78E_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
	
	glm::ivec2 Window::Resolution::resolution(Resolution resolution) {
		C78E_CORE_VALIDATE(resolution.m_Resolution < 25, return s_Resolutions[FHD], "Window::Resolution::resolution: resolution was illegal!")
		return s_Resolutions[resolution.m_Resolution];
	}

	std::string Window::Resolution::resolutionToString(const Resolution& resolution) {
		switch (resolution.m_Resolution) {
		case C78E::Window::Resolution::nHD:		return "Resolution::nHD(640x360)";
		case C78E::Window::Resolution::VGA:		return "Resolution::VGA(640x480)";
		case C78E::Window::Resolution::SVGA:	return "Resolution::SVGA(800x600)";
		case C78E::Window::Resolution::XGA:		return "Resolution::XGA(1024x768)";
		case C78E::Window::Resolution::WXGA:	return "Resolution::WXGA(1280x720)";
		case C78E::Window::Resolution::WXGAe:	return "Resolution::WXGAe(1280x800)";
		case C78E::Window::Resolution::SXGA:	return "Resolution::SXGA(1280x1024)";
		case C78E::Window::Resolution::HD:		return "Resolution::HD(1360x768)";
		case C78E::Window::Resolution::HDe:		return "Resolution::HDe(1366x768)";
		case C78E::Window::Resolution::WXGAp:	return "Resolution::WXGAp(1440x900)";
		case C78E::Window::Resolution::HDp:		return "Resolution::HDp(1600x900)";
		case C78E::Window::Resolution::UXGA:	return "Resolution::UXGA(1600x1200)";
		case C78E::Window::Resolution::WSXGAp:	return "Resolution::WSXGAp(1680x1050)";
		case C78E::Window::Resolution::FHD:		return "Resolution::FHD(1920x1080)";
		case C78E::Window::Resolution::WUXGA:	return "Resolution::WUXGA(1920x1200)";
		case C78E::Window::Resolution::QWXGA:	return "Resolution::QWXGA(2048x1152)";
		case C78E::Window::Resolution::QXGA:	return "Resolution::QXGA(2048x1536)";
		case C78E::Window::Resolution::UWFHD:	return "Resolution::UWFHD(2560x1080)";
		case C78E::Window::Resolution::QHD:		return "Resolution::QHD(2560x1440)";
		case C78E::Window::Resolution::WQXGA:	return "Resolution::WQXGA(2560x1600)";
		case C78E::Window::Resolution::UWQHD:	return "Resolution::UWQHD(3440x1440)";
		case C78E::Window::Resolution::UHD_4K:	return "Resolution::UHD_4K(3840x2160)";
		case C78E::Window::Resolution::UHD_5K:	return "Resolution::UHD_5K(5120x2880)";
		case C78E::Window::Resolution::DUHD:	return "Resolution::DUHD(7680x2160)";
		case C78E::Window::Resolution::UHD_8K:	return "Resolution::UHD_8K(7680x4320)";
		default: return "Resolution::None";
		}
	}

	Window::Resolution Window::Resolution::resolutionFromString(const std::string& resolutionString) {
		if ("Resolution::nHD(640x360)")			return C78E::Window::Resolution::nHD;
		if ("Resolution::VGA(640x480)")			return C78E::Window::Resolution::VGA;
		if ("Resolution::SVGA(800x600)")		return C78E::Window::Resolution::SVGA;
		if ("Resolution::XGA(1024x768)")		return C78E::Window::Resolution::XGA;
		if ("Resolution::WXGA(1280x720)")		return C78E::Window::Resolution::WXGA;
		if ("Resolution::WXGAe(1280x800)")		return C78E::Window::Resolution::WXGAe;
		if ("Resolution::SXGA(1280x1024)")		return C78E::Window::Resolution::SXGA;
		if ("Resolution::HD(1360x768)")			return C78E::Window::Resolution::HD;
		if ("Resolution::HDe(1366x768)")		return C78E::Window::Resolution::HDe;
		if ("Resolution::WXGAp(1440x900)")		return C78E::Window::Resolution::WXGAp;
		if ("Resolution::HDp(1600x900)")		return C78E::Window::Resolution::HDp;
		if ("Resolution::UXGA(1600x1200)")		return C78E::Window::Resolution::UXGA;
		if ("Resolution::WSXGAp(1680x1050)")	return C78E::Window::Resolution::WSXGAp;
		if ("Resolution::FHD(1920x1080)")		return C78E::Window::Resolution::FHD;
		if ("Resolution::WUXGA(1920x1200)")		return C78E::Window::Resolution::WUXGA;
		if ("Resolution::QWXGA(2048x1152)")		return C78E::Window::Resolution::QWXGA;
		if ("Resolution::QXGA(2048x1536)")		return C78E::Window::Resolution::QXGA;
		if ("Resolution::UWFHD(2560x1080)")		return C78E::Window::Resolution::UWFHD;
		if ("Resolution::QHD(2560x1440)")		return C78E::Window::Resolution::QHD;
		if ("Resolution::WQXGA(2560x1600)")		return C78E::Window::Resolution::WQXGA;
		if ("Resolution::UWQHD(3440x1440)")		return C78E::Window::Resolution::UWQHD;
		if ("Resolution::UHD_4K(3840x2160)")	return C78E::Window::Resolution::UHD_4K;
		if ("Resolution::UHD_5K(5120x2880)")	return C78E::Window::Resolution::UHD_5K;
		if ("Resolution::DUHD(7680x2160)")		return C78E::Window::Resolution::DUHD;
		if ("Resolution::UHD_8K(7680x4320)")	return C78E::Window::Resolution::UHD_8K;
		return C78E::Window::Resolution::FHD;
	}

	const std::array<glm::ivec2, 25>& Window::Resolution::resolutions() {
		return s_Resolutions;
	}

	const std::array<uint8_t, 25>& Window::Resolution::resolutionTypes() {
		return { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	}

}