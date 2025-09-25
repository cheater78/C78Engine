#pragma once

namespace C78E {

	class ColorSpace {
	public:
		enum ColorSpaceType {
			sRGB,
			sRGBL,
		};
	public:
		static std::string colorSpaceToString(const ColorSpace& colorSpace);
		static ColorSpace colorSpaceFromString(const std::string& colorSpaceString);
		static bool isLinear(const ColorSpace& colorSpace);
	public:
		ColorSpace();
		ColorSpace(ColorSpaceType colorSpace);
		~ColorSpace();

		ColorSpaceType raw() const;

		bool operator==(const ColorSpace& other) const;
		bool operator!=(const ColorSpace& other) const;
	private:
		ColorSpaceType m_ColorSpace = ColorSpaceType::sRGB;
	};

}

namespace std {

	_EXPORT_STD _NODISCARD inline string to_string(const C78E::ColorSpace& colorSpace) {
		return C78E::ColorSpace::colorSpaceToString(colorSpace);
	}

}

template<>
DefineFormatter(C78E::ColorSpace, std::to_string(input));
