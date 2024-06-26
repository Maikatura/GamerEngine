#pragma once
#include <cstdint>

namespace CommonUtilities
{
	using InputCode = uint16_t;
	static constexpr InputCode InvalidInputCode = 0;

	struct Key
	{
		enum Code : InputCode
		{
			None = InvalidInputCode,
			Tab,
			LeftArrow,
			RightArrow,
			UpArrow,
			DownArrow,
			PageUp,
			PageDown,
			Home,
			End,
			Insert,
			Delete,
			Backspace,
			Space,
			Enter,
			Escape,
			Apostrophe,
			Comma,
			Minus,
			Period,
			Slash,
			Semicolon,
			Equal,
			LeftBracket,
			Backslash,
			RightBracket,
			GraveAccent,
			CapsLock,
			ScrollLock,
			NumLock,
			PrintScreen,
			Pause,
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			NumpadDecimal,
			NumpadDivide,
			NumpadMultiply,
			NumpadSubtract,
			NumpadAdd,
			NumpadEnter,
			LeftShift,
			LeftCtrl,
			LeftAlt,
			LeftSuper,
			RightShift,
			RightCtrl,
			RightAlt,
			RightSuper,
			Menu,
			N0,
			N1,
			N2,
			N3,
			N4,
			N5,
			N6,
			N7,
			N8,
			N9,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			KeysCount
		};
	};

	struct Mouse
	{
		enum Button : InputCode
		{
			None = InvalidInputCode,
			LeftButton,
			RightButton,
			MiddleButton,
			X1Button,
			X2Button,
			ButtonsCount,
		};

		enum Axis : InputCode
		{
			MouseMovement,
			ScrollWheel,
			AxisCount
		};
	};

	struct Xbox
	{
		enum Button : InputCode
		{
			None = InvalidInputCode,
			Up,
			Down,
			Left,
			Right,
			Start,
			Back,
			LeftStick,
			RightStick,
			LeftStickUp,
			LeftStickDown,
			LeftStickRight,
			LeftStickLeft,
			RightStickUp,
			RightStickDown,
			RightStickRight,
			RightStickLeft,
			LeftBumper,
			RightBumper,
			LeftTrigger,
			RightTrigger,
			A,
			B,
			X,
			Y,
			ButtonsCount
		};

	};

}
