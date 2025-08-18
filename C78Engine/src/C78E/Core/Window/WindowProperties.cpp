#include "C78EPCH.h"
#include "WindowProperties.h"

namespace C78E {

	const std::array<ivec2, 25> Resolution::s_Resolutions = {
		ivec2{ 640,  360},
		ivec2{ 640,  480},
		ivec2{ 800,  600},
		ivec2{1024,  768},
		ivec2{1280,  720},
		ivec2{1280,  800},
		ivec2{1280, 1024},
		ivec2{1360,  768},
		ivec2{1366,  768},
		ivec2{1440,  900},
		ivec2{1600,  900},
		ivec2{1600, 1200},
		ivec2{1680, 1050},
		ivec2{1920, 1080},
		ivec2{1920, 1200},
		ivec2{2048, 1152},
		ivec2{2048, 1536},
		ivec2{2560, 1080},
		ivec2{2560, 1440},
		ivec2{2560, 1600},
		ivec2{3440, 1440},
		ivec2{3840, 2160},
		ivec2{5120, 2880},
		ivec2{7680, 2160},
		ivec2{7680, 4320}
	};

	ivec2 Resolution::resolution(Resolution resolution) {
		C78E_CORE_VALIDATE(resolution.m_Resolution < 25, return s_Resolutions[FHD], "Window::Resolution::resolution: resolution was illegal!")
			return s_Resolutions[resolution.m_Resolution];
	}

	std::string Resolution::resolutionToString(const Resolution& resolution) {
		switch (resolution.m_Resolution) {
		case C78E::Resolution::nHD:		return "Resolution::nHD(640x360)";
		case C78E::Resolution::VGA:		return "Resolution::VGA(640x480)";
		case C78E::Resolution::SVGA:	return "Resolution::SVGA(800x600)";
		case C78E::Resolution::XGA:		return "Resolution::XGA(1024x768)";
		case C78E::Resolution::WXGA:	return "Resolution::WXGA(1280x720)";
		case C78E::Resolution::WXGAe:	return "Resolution::WXGAe(1280x800)";
		case C78E::Resolution::SXGA:	return "Resolution::SXGA(1280x1024)";
		case C78E::Resolution::HD:		return "Resolution::HD(1360x768)";
		case C78E::Resolution::HDe:		return "Resolution::HDe(1366x768)";
		case C78E::Resolution::WXGAp:	return "Resolution::WXGAp(1440x900)";
		case C78E::Resolution::HDp:		return "Resolution::HDp(1600x900)";
		case C78E::Resolution::UXGA:	return "Resolution::UXGA(1600x1200)";
		case C78E::Resolution::WSXGAp:	return "Resolution::WSXGAp(1680x1050)";
		case C78E::Resolution::FHD:		return "Resolution::FHD(1920x1080)";
		case C78E::Resolution::WUXGA:	return "Resolution::WUXGA(1920x1200)";
		case C78E::Resolution::QWXGA:	return "Resolution::QWXGA(2048x1152)";
		case C78E::Resolution::QXGA:	return "Resolution::QXGA(2048x1536)";
		case C78E::Resolution::UWFHD:	return "Resolution::UWFHD(2560x1080)";
		case C78E::Resolution::QHD:		return "Resolution::QHD(2560x1440)";
		case C78E::Resolution::WQXGA:	return "Resolution::WQXGA(2560x1600)";
		case C78E::Resolution::UWQHD:	return "Resolution::UWQHD(3440x1440)";
		case C78E::Resolution::UHD_4K:	return "Resolution::UHD_4K(3840x2160)";
		case C78E::Resolution::UHD_5K:	return "Resolution::UHD_5K(5120x2880)";
		case C78E::Resolution::DUHD:	return "Resolution::DUHD(7680x2160)";
		case C78E::Resolution::UHD_8K:	return "Resolution::UHD_8K(7680x4320)";
		default: return "Resolution::None";
		}
	}

	Resolution Resolution::resolutionFromString(const std::string& resolutionString) {
		if ("Resolution::nHD(640x360)")			return C78E::Resolution::nHD;
		if ("Resolution::VGA(640x480)")			return C78E::Resolution::VGA;
		if ("Resolution::SVGA(800x600)")		return C78E::Resolution::SVGA;
		if ("Resolution::XGA(1024x768)")		return C78E::Resolution::XGA;
		if ("Resolution::WXGA(1280x720)")		return C78E::Resolution::WXGA;
		if ("Resolution::WXGAe(1280x800)")		return C78E::Resolution::WXGAe;
		if ("Resolution::SXGA(1280x1024)")		return C78E::Resolution::SXGA;
		if ("Resolution::HD(1360x768)")			return C78E::Resolution::HD;
		if ("Resolution::HDe(1366x768)")		return C78E::Resolution::HDe;
		if ("Resolution::WXGAp(1440x900)")		return C78E::Resolution::WXGAp;
		if ("Resolution::HDp(1600x900)")		return C78E::Resolution::HDp;
		if ("Resolution::UXGA(1600x1200)")		return C78E::Resolution::UXGA;
		if ("Resolution::WSXGAp(1680x1050)")	return C78E::Resolution::WSXGAp;
		if ("Resolution::FHD(1920x1080)")		return C78E::Resolution::FHD;
		if ("Resolution::WUXGA(1920x1200)")		return C78E::Resolution::WUXGA;
		if ("Resolution::QWXGA(2048x1152)")		return C78E::Resolution::QWXGA;
		if ("Resolution::QXGA(2048x1536)")		return C78E::Resolution::QXGA;
		if ("Resolution::UWFHD(2560x1080)")		return C78E::Resolution::UWFHD;
		if ("Resolution::QHD(2560x1440)")		return C78E::Resolution::QHD;
		if ("Resolution::WQXGA(2560x1600)")		return C78E::Resolution::WQXGA;
		if ("Resolution::UWQHD(3440x1440)")		return C78E::Resolution::UWQHD;
		if ("Resolution::UHD_4K(3840x2160)")	return C78E::Resolution::UHD_4K;
		if ("Resolution::UHD_5K(5120x2880)")	return C78E::Resolution::UHD_5K;
		if ("Resolution::DUHD(7680x2160)")		return C78E::Resolution::DUHD;
		if ("Resolution::UHD_8K(7680x4320)")	return C78E::Resolution::UHD_8K;
		return C78E::Resolution::FHD;
	}

	const std::array<ivec2, 25>& Resolution::resolutions() {
		return s_Resolutions;
	}

	const std::array<uint8_t, 25>& Resolution::resolutionTypes() {
		static const std::array<uint8_t, 25> resolutionTypes{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
		return resolutionTypes;
	}

}