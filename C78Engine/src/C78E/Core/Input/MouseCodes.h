#pragma once
#include <C78E/Utils/StdUtils.h>

namespace C78E::Input {
	using MouseCode = uint16_t;

	namespace Mouse {
		enum : MouseCode {
			Button0                = 0,
			Button1                = 1,
			Button2                = 2,
			Button3                = 3,
			Button4                = 4,
			Button5                = 5,
			Button6                = 6,
			Button7                = 7,

			ButtonLast             = Button7,
			ButtonLeft             = Button0,
			ButtonRight            = Button1,
			ButtonMiddle           = Button2,

			None				   = (uint16_t)-1,
		};

		static MouseCode mousCodeFromString(const std::string& str) {
			if(str == "MouseLeft") return Mouse::ButtonLeft;
			if(str == "MouseRight") return Mouse::ButtonRight;
			if(str == "MouseMiddle") return Mouse::ButtonMiddle;
			if(str == "Mouse3") return Mouse::Button3;
			if(str == "Mouse4") return Mouse::Button4;
			if(str == "Mouse5") return Mouse::Button5;
			if(str == "Mouse6") return Mouse::Button6;
			if(str == "Mouse7") return Mouse::Button7;
			return Mouse::None;
		}

		static std::string mouseCodeToString(MouseCode key) {
			switch (key) {
			case Mouse::ButtonLeft: return "MouseLeft";
			case Mouse::ButtonRight: return "MouseRight";
			case Mouse::ButtonMiddle: return "MouseMiddle";
			case Mouse::Button3: return "Mouse3";
			case Mouse::Button4: return "Mouse4";
			case Mouse::Button5: return "Mouse5";
			case Mouse::Button6: return "Mouse6";
			case Mouse::Button7: return "Mouse7";
			default: return "None";
			}
		}
	}
}