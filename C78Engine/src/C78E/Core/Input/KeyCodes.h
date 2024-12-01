#pragma once
#include <C78E/Utils/StdUtils.h>

namespace C78E::Input {
	using KeyCode = uint16_t;
	namespace Key {
		enum : KeyCode {
			None				= 0,
			// From glfw3.h
			Space               = 32,
			Apostrophe          = 39, /* ' */
			Comma               = 44, /* , */
			Minus               = 45, /* - */
			Period              = 46, /* . */
			Slash               = 47, /* / */

			D0                  = 48, /* 0 */
			D1                  = 49, /* 1 */
			D2                  = 50, /* 2 */
			D3                  = 51, /* 3 */
			D4                  = 52, /* 4 */
			D5                  = 53, /* 5 */
			D6                  = 54, /* 6 */
			D7                  = 55, /* 7 */
			D8                  = 56, /* 8 */
			D9                  = 57, /* 9 */

			Semicolon           = 59, /* ; */
			Equal               = 61, /* = */

			A                   = 65,
			B                   = 66,
			C                   = 67,
			D                   = 68,
			E                   = 69,
			F                   = 70,
			G                   = 71,
			H                   = 72,
			I                   = 73,
			J                   = 74,
			K                   = 75,
			L                   = 76,
			M                   = 77,
			N                   = 78,
			O                   = 79,
			P                   = 80,
			Q                   = 81,
			R                   = 82,
			S                   = 83,
			T                   = 84,
			U                   = 85,
			V                   = 86,
			W                   = 87,
			X                   = 88,
			Y                   = 89,
			Z                   = 90,

			LeftBracket         = 91,  /* [ */
			Backslash           = 92,  /* \ */
			RightBracket        = 93,  /* ] */
			GraveAccent         = 96,  /* ` */

			World1              = 161, /* non-US #1 */
			World2              = 162, /* non-US #2 */

			/* Function keys */
			Escape              = 256,
			Enter               = 257,
			Tab                 = 258,
			Backspace           = 259,
			Insert              = 260,
			Delete              = 261,
			Right               = 262,
			Left                = 263,
			Down                = 264,
			Up                  = 265,
			PageUp              = 266,
			PageDown            = 267,
			Home                = 268,
			End                 = 269,
			CapsLock            = 280,
			ScrollLock          = 281,
			NumLock             = 282,
			PrintScreen         = 283,
			Pause               = 284,
			F1                  = 290,
			F2                  = 291,
			F3                  = 292,
			F4                  = 293,
			F5                  = 294,
			F6                  = 295,
			F7                  = 296,
			F8                  = 297,
			F9                  = 298,
			F10                 = 299,
			F11                 = 300,
			F12                 = 301,
			F13                 = 302,
			F14                 = 303,
			F15                 = 304,
			F16                 = 305,
			F17                 = 306,
			F18                 = 307,
			F19                 = 308,
			F20                 = 309,
			F21                 = 310,
			F22                 = 311,
			F23                 = 312,
			F24                 = 313,
			F25                 = 314,

			/* Keypad */
			KP0                 = 320,
			KP1                 = 321,
			KP2                 = 322,
			KP3                 = 323,
			KP4                 = 324,
			KP5                 = 325,
			KP6                 = 326,
			KP7                 = 327,
			KP8                 = 328,
			KP9                 = 329,
			KPDecimal           = 330,
			KPDivide            = 331,
			KPMultiply          = 332,
			KPSubtract          = 333,
			KPAdd               = 334,
			KPEnter             = 335,
			KPEqual             = 336,

			LeftShift           = 340,
			LeftControl         = 341,
			LeftAlt             = 342,
			LeftSuper           = 343,
			RightShift          = 344,
			RightControl        = 345,
			RightAlt            = 346,
			RightSuper          = 347,
			Menu                = 348

		};

		static KeyCode keyCodeFromString(const std::string& str) {
			if(str == "Space") return Key::Space;
			if(str == "Apostrophe") return Key::Apostrophe;
			if(str == "Comma") return Key::Comma;
			if(str == "Minus") return Key::Minus;
			if(str == "Period") return Key::Period;
			if(str == "Slash") return Key::Slash;
			if(str == "0") return Key::D0;
			if(str == "1") return Key::D1;
			if(str == "2") return Key::D2;
			if(str == "3") return Key::D3;
			if(str == "4") return Key::D4;
			if(str == "5") return Key::D5;
			if(str == "6") return Key::D6;
			if(str == "7") return Key::D7;
			if(str == "8") return Key::D8;
			if(str == "9") return Key::D9;
			if(str == "Semicolon") return Key::Semicolon;
			if(str == "Equal") return Key::Equal;
			if(str == "A") return Key::A;
			if(str == "B") return Key::B;
			if(str == "C") return Key::C;
			if(str == "D") return Key::D;
			if(str == "E") return Key::E;
			if(str == "F") return Key::F;
			if(str == "G") return Key::G;
			if(str == "H") return Key::H;
			if(str == "I") return Key::I;
			if(str == "J") return Key::J;
			if(str == "K") return Key::K;
			if(str == "L") return Key::L;
			if(str == "M") return Key::M;
			if(str == "N") return Key::N;
			if(str == "O") return Key::O;
			if(str == "P") return Key::P;
			if(str == "Q") return Key::Q;
			if(str == "R") return Key::R;
			if(str == "S") return Key::S;
			if(str == "T") return Key::T;
			if(str == "U") return Key::U;
			if(str == "V") return Key::V;
			if(str == "W") return Key::W;
			if(str == "X") return Key::X;
			if(str == "Y") return Key::Y;
			if(str == "Z") return Key::Z;
			if(str == "LeftBracket") return Key::LeftBracket;
			if(str == "Backslash") return Key::Backslash;
			if(str == "RightBracket") return Key::RightBracket;
			if(str == "GraveAccent") return Key::GraveAccent;
			if(str == "World1") return Key::World1;
			if(str == "World2") return Key::World2;
			if(str == "Escape") return Key::Escape;
			if(str == "Enter") return Key::Enter;
			if(str == "Tab") return Key::Tab;
			if(str == "Backspace") return Key::Backspace;
			if(str == "Insert") return Key::Insert;
			if(str == "Delete") return Key::Delete;
			if(str == "Right") return Key::Right;
			if(str == "Left") return Key::Left;
			if(str == "Down") return Key::Down;
			if(str == "Up") return Key::Up;
			if(str == "PageUp") return Key::PageUp;
			if(str == "PageDown") return Key::PageDown;
			if(str == "Home") return Key::Home;
			if(str == "End") return Key::End;
			if(str == "CapsLock") return Key::CapsLock;
			if(str == "ScrollLock") return Key::ScrollLock;
			if(str == "NumLock") return Key::NumLock;
			if(str == "PrintScreen") return Key::PrintScreen;
			if(str == "Pause") return Key::Pause;
			if(str == "F1") return Key::F1;
			if(str == "F2") return Key::F1;
			if(str == "F3") return Key::F2;
			if(str == "F4") return Key::F3;
			if(str == "F5") return Key::F4;
			if(str == "F6") return Key::F5;
			if(str == "F7") return Key::F6;
			if(str == "F8") return Key::F7;
			if(str == "F9") return Key::F8;
			if(str == "F10") return Key::F10;
			if(str == "F11") return Key::F11;
			if(str == "F12") return Key::F12;
			if(str == "F13") return Key::F13;
			if(str == "F14") return Key::F14;
			if(str == "F15") return Key::F15;
			if(str == "F16") return Key::F16;
			if(str == "F17") return Key::F17;
			if(str == "F18") return Key::F18;
			if(str == "F19") return Key::F19;
			if(str == "F20") return Key::F20;
			if(str == "F21") return Key::F21;
			if(str == "F22") return Key::F22;
			if(str == "F23") return Key::F23;
			if(str == "F24") return Key::F24;
			if(str == "F25") return Key::F25;
			if(str == "Num0") return Key::KP0;
			if(str == "Num1") return Key::KP1;
			if(str == "Num2") return Key::KP2;
			if(str == "Num3") return Key::KP3;
			if(str == "Num4") return Key::KP4;
			if(str == "Num5") return Key::KP5;
			if(str == "Num6") return Key::KP6;
			if(str == "Num7") return Key::KP7;
			if(str == "Num8") return Key::KP8;
			if(str == "Num9") return Key::KP9;
			if(str == "NumDecimal") return Key::KPDecimal;
			if(str == "NumDivide") return Key::KPDivide;
			if(str == "NumMultiply") return Key::KPMultiply;
			if(str == "NumSubtract") return Key::KPSubtract;
			if(str == "NumAdd") return Key::KPAdd;
			if(str == "NumEnter") return Key::KPEnter;
			if(str == "NumEqual") return Key::KPEqual;
			if(str == "LeftShift") return Key::LeftShift;
			if(str == "LeftControl") return Key::LeftControl;
			if(str == "LeftAlt") return Key::LeftAlt;
			if(str == "LeftSuper") return Key::LeftSuper;
			if(str == "RightShift") return Key::RightShift;
			if(str == "RightControl") return Key::RightControl;
			if(str == "RightAlt") return Key::RightAlt;
			if(str == "RightSuper") return Key::RightSuper;
			if(str == "Menu") return Key::Menu;
			return Key::None;
		}

		static std::string keyCodeToString(KeyCode key) {
			switch (key) {
			case Key::Space: return "Space";
			case Key::Apostrophe: return "Apostrophe";
			case Key::Comma: return "Comma";
			case Key::Minus: return "Minus";
			case Key::Period: return "Period";
			case Key::Slash: return "Slash";
			case Key::D0: return "0";
			case Key::D1: return "1";
			case Key::D2: return "2";
			case Key::D3: return "3";
			case Key::D4: return "4";
			case Key::D5: return "5";
			case Key::D6: return "6";
			case Key::D7: return "7";
			case Key::D8: return "8";
			case Key::D9: return "9";
			case Key::Semicolon: return "Semicolon";
			case Key::Equal: return "Equal";
			case Key::A: return "A";
			case Key::B: return "B";
			case Key::C: return "C";
			case Key::D: return "D";
			case Key::E: return "E";
			case Key::F: return "F";
			case Key::G: return "G";
			case Key::H: return "H";
			case Key::I: return "I";
			case Key::J: return "J";
			case Key::K: return "K";
			case Key::L: return "L";
			case Key::M: return "M";
			case Key::N: return "N";
			case Key::O: return "O";
			case Key::P: return "P";
			case Key::Q: return "Q";
			case Key::R: return "R";
			case Key::S: return "S";
			case Key::T: return "T";
			case Key::U: return "U";
			case Key::V: return "V";
			case Key::W: return "W";
			case Key::X: return "X";
			case Key::Y: return "Y";
			case Key::Z: return "Z";
			case Key::LeftBracket: return "LeftBracket";
			case Key::Backslash: return "Backslash";
			case Key::RightBracket: return "RightBracket";
			case Key::GraveAccent: return "GraveAccent";
			case Key::World1: return "World1";
			case Key::World2: return "World2";
			case Key::Escape: return "Escape";
			case Key::Enter: return "Enter";
			case Key::Tab: return "Tab";
			case Key::Backspace: return "Backspace";
			case Key::Insert: return "Insert";
			case Key::Delete: return "Delete";
			case Key::Right: return "Right";
			case Key::Left: return "Left";
			case Key::Down: return "Down";
			case Key::Up: return "Up";
			case Key::PageUp: return "PageUp";
			case Key::PageDown: return "PageDown";
			case Key::Home: return "Home";
			case Key::End: return "End";
			case Key::CapsLock: return "CapsLock";
			case Key::ScrollLock: return "ScrollLock";
			case Key::NumLock: return "NumLock";
			case Key::PrintScreen: return "PrintScreen";
			case Key::Pause: return "Pause";
			case Key::F1: return "F1";
			case Key::F2: return "F2";
			case Key::F3: return "F3";
			case Key::F4: return "F4";
			case Key::F5: return "F5";
			case Key::F6: return "F6";
			case Key::F7: return "F7";
			case Key::F8: return "F8";
			case Key::F9: return "F9";
			case Key::F10: return "F10";
			case Key::F11: return "F11";
			case Key::F12: return "F12";
			case Key::F13: return "F13";
			case Key::F14: return "F14";
			case Key::F15: return "F15";
			case Key::F16: return "F16";
			case Key::F17: return "F17";
			case Key::F18: return "F18";
			case Key::F19: return "F19";
			case Key::F20: return "F20";
			case Key::F21: return "F21";
			case Key::F22: return "F22";
			case Key::F23: return "F23";
			case Key::F24: return "F24";
			case Key::F25: return "F25";
			case Key::KP0: return "Num0";
			case Key::KP1: return "Num1";
			case Key::KP2: return "Num2";
			case Key::KP3: return "Num3";
			case Key::KP4: return "Num4";
			case Key::KP5: return "Num5";
			case Key::KP6: return "Num6";
			case Key::KP7: return "Num7";
			case Key::KP8: return "Num8";
			case Key::KP9: return "Num9";
			case Key::KPDecimal: return "NumDecimal";
			case Key::KPDivide: return "NumDivide";
			case Key::KPMultiply: return "NumMultiply";
			case Key::KPSubtract: return "NumSubtract";
			case Key::KPAdd: return "NumAdd";
			case Key::KPEnter: return "NumEnter";
			case Key::KPEqual: return "NumEqual";
			case Key::LeftShift: return "LeftShift";
			case Key::LeftControl: return "LeftControl";
			case Key::LeftAlt: return "LeftAlt";
			case Key::LeftSuper: return "LeftSuper";
			case Key::RightShift: return "RightShift";
			case Key::RightControl: return "RightControl";
			case Key::RightAlt: return "RightAlt";
			case Key::RightSuper: return "RightSuper";
			case Key::Menu: return "Menu";
			default: return "None";
			}
		}
	}
}