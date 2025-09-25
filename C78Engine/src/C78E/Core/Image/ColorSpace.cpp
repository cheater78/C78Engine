#include "C78EPCH.h"
#include "ColorSpace.h"

namespace C78E {

	std::string ColorSpace::colorSpaceToString(const ColorSpace& colorSpace) {
		switch (colorSpace.raw()) {
		case sRGB:		return "ColorSpace::sRGB";
		case sRGBL:		return "ColorSpace::sRGBL";
		default:		return "ColorSpace::sRGB";
		}
	}

	ColorSpace ColorSpace::colorSpaceFromString(const std::string& colorSpaceString) {
		if (colorSpaceString == "ColorSpace::sRGB") return sRGB;
		if (colorSpaceString == "ColorSpace::sRGBL") return sRGBL;
		return sRGB;
	}

	bool ColorSpace::isLinear(const ColorSpace& colorSpace) {
		switch (colorSpace.raw()) {
		case sRGB:		return false;
		case sRGBL:		return true;
		default:		return false;
		}
	}

	ColorSpace::ColorSpace()
		: m_ColorSpace(ColorSpaceType::sRGB) {
	}

	ColorSpace::ColorSpace(ColorSpaceType colorSpace)
		: m_ColorSpace(colorSpace) {
	}

	ColorSpace::~ColorSpace() {
	}

	ColorSpace::ColorSpaceType ColorSpace::raw() const {
		return m_ColorSpace;
	}

	bool ColorSpace::operator==(const ColorSpace& other) const {
		return raw() == other.raw();
	}

	bool ColorSpace::operator!=(const ColorSpace& other) const {
		return !operator==(other);
	}

}
