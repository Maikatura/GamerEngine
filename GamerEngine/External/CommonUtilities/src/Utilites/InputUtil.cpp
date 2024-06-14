#include "pch.h"
#include "InputUtil.h"
#include <xinput.h>
#include "Asserts.h"

namespace CommonUtilities::Internal
{
	constexpr CommonUtilities::Key::Code WindowsVKToCUKey(int aWindowsVK)
	{
		switch (aWindowsVK)
		{
		case VK_TAB:
			return Key::Tab;
		case VK_LEFT:
			return Key::LeftArrow;
		case VK_RIGHT:
			return Key::RightArrow;
		case VK_UP:
			return Key::UpArrow;
		case VK_DOWN:
			return Key::DownArrow;
		case VK_PRIOR:
			return Key::PageUp;
		case VK_NEXT:
			return Key::PageDown;
		case VK_HOME:
			return Key::Home;
		case VK_END:
			return Key::End;
		case VK_INSERT:
			return Key::Insert;
		case VK_DELETE:
			return Key::Delete;
		case VK_BACK:
			return Key::Backspace;
		case VK_SPACE:
			return Key::Space;
		case VK_RETURN:
			return Key::Enter;
		case VK_ESCAPE:
			return Key::Escape;
		case VK_OEM_7:
			return Key::Apostrophe;
		case VK_OEM_COMMA:
			return Key::Comma;
		case VK_OEM_MINUS:
			return Key::Minus;
		case VK_OEM_PERIOD:
			return Key::Period;
		case VK_OEM_2:
			return Key::Slash;
		case VK_OEM_1:
			return Key::Semicolon;
		case VK_OEM_PLUS:
			return Key::Equal;
		case VK_OEM_4:
			return Key::LeftBracket;
		case VK_OEM_5:
			return Key::Backslash;
		case VK_OEM_6:
			return Key::RightBracket;
		case VK_OEM_3:
			return Key::GraveAccent;
		case VK_CAPITAL:
			return Key::CapsLock;
		case VK_SCROLL:
			return Key::ScrollLock;
		case VK_NUMLOCK:
			return Key::NumLock;
		case VK_SNAPSHOT:
			return Key::PrintScreen;
		case VK_PAUSE:
			return Key::Pause;
		case VK_NUMPAD0:
			return Key::Numpad0;
		case VK_NUMPAD1:
			return Key::Numpad1;
		case VK_NUMPAD2:
			return Key::Numpad2;
		case VK_NUMPAD3:
			return Key::Numpad3;
		case VK_NUMPAD4:
			return Key::Numpad4;
		case VK_NUMPAD5:
			return Key::Numpad5;
		case VK_NUMPAD6:
			return Key::Numpad6;
		case VK_NUMPAD7:
			return Key::Numpad7;
		case VK_NUMPAD8:
			return Key::Numpad8;
		case VK_NUMPAD9:
			return Key::Numpad9;
		case VK_DECIMAL:
			return Key::NumpadDecimal;
		case VK_DIVIDE:
			return Key::NumpadDivide;
		case VK_MULTIPLY:
			return Key::NumpadMultiply;
		case VK_SUBTRACT:
			return Key::NumpadSubtract;
		case VK_ADD:
			return Key::NumpadAdd;
		case (VK_RETURN + KF_EXTENDED):
			return Key::NumpadEnter;
		case VK_LSHIFT:
			return Key::LeftShift;
		case VK_LCONTROL:
			return Key::LeftCtrl;
		case VK_LMENU:
			return Key::LeftAlt;
		case VK_LWIN:
			return Key::LeftSuper;
		case VK_RSHIFT:
			return Key::RightShift;
		case VK_RCONTROL:
			return Key::RightCtrl;
		case VK_RMENU:
			return Key::RightAlt;
		case VK_RWIN:
			return Key::RightSuper;
		case VK_APPS:
			return Key::Menu;
		case '0':
			return Key::N0;
		case '1':
			return Key::N1;
		case '2':
			return Key::N2;
		case '3':
			return Key::N3;
		case '4':
			return Key::N4;
		case '5':
			return Key::N5;
		case '6':
			return Key::N6;
		case '7':
			return Key::N7;
		case '8':
			return Key::N8;
		case '9':
			return Key::N9;
		case 'A':
			return Key::A;
		case 'B':
			return Key::B;
		case 'C':
			return Key::C;
		case 'D':
			return Key::D;
		case 'E':
			return Key::E;
		case 'F':
			return Key::F;
		case 'G':
			return Key::G;
		case 'H':
			return Key::H;
		case 'I':
			return Key::I;
		case 'J':
			return Key::J;
		case 'K':
			return Key::K;
		case 'L':
			return Key::L;
		case 'M':
			return Key::M;
		case 'N':
			return Key::N;
		case 'O':
			return Key::O;
		case 'P':
			return Key::P;
		case 'Q':
			return Key::Q;
		case 'R':
			return Key::R;
		case 'S':
			return Key::S;
		case 'T':
			return Key::T;
		case 'U':
			return Key::U;
		case 'V':
			return Key::V;
		case 'W':
			return Key::W;
		case 'X':
			return Key::X;
		case 'Y':
			return Key::Y;
		case 'Z':
			return Key::Z;
		case VK_F1:
			return Key::F1;
		case VK_F2:
			return Key::F2;
		case VK_F3:
			return Key::F3;
		case VK_F4:
			return Key::F4;
		case VK_F5:
			return Key::F5;
		case VK_F6:
			return Key::F6;
		case VK_F7:
			return Key::F7;
		case VK_F8:
			return Key::F8;
		case VK_F9:
			return Key::F9;
		case VK_F10:
			return Key::F10;
		case VK_F11:
			return Key::F11;
		case VK_F12:
			return Key::F12;
		default:
			return Key::None;
		}
	}
}

CommonUtilities::Key::Code CommonUtilities::WindowsVKToCUKey(int aVK, WORD aLParamHiWord)
{
	switch (aVK)
	{
	case VK_RETURN:
		if (aLParamHiWord & KF_EXTENDED)
		{
			aVK = (VK_RETURN + KF_EXTENDED); //Numpad Enter (has no VK)
		}
		break;

	case VK_SHIFT:
		if (aLParamHiWord & KF_EXTENDED)
		{
			aVK = VK_RSHIFT;
		}
		else
		{
			aVK = VK_LSHIFT;
		}
		break;

	case VK_CONTROL:
		if (aLParamHiWord & KF_EXTENDED)
		{
			aVK = VK_RCONTROL;
		}
		else
		{
			aVK = VK_LCONTROL;
		}
		break;

	case VK_MENU:
		if (aLParamHiWord & KF_EXTENDED)
		{
			aVK = VK_RMENU;
		}
		else
		{
			aVK = VK_LMENU;
		}
		break;

	default:
		break;
	}

	return Internal::WindowsVKToCUKey(aVK);
}

CommonUtilities::Mouse::Button CommonUtilities::XButtonFromWParam(WPARAM aWParam)
{
	int xButton = GET_XBUTTON_WPARAM(aWParam);
	if (xButton == XBUTTON1)
	{
		return Mouse::X1Button;
	}
	else if (xButton == XBUTTON2)
	{
		return Mouse::X2Button;
	}

	GE_ASSERT(false, "Invalid WParam passed");
	return Mouse::None;
}

std::bitset<CommonUtilities::Xbox::ButtonsCount> CommonUtilities::XInputToCUButtons(WORD aXInputButtons)
{
	std::bitset<Xbox::ButtonsCount> state;
	if (aXInputButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		state.set(Xbox::Up);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		state.set(Xbox::Down);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		state.set(Xbox::Left);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		state.set(Xbox::Right);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_START)
	{
		state.set(Xbox::Start);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_BACK)
	{
		state.set(Xbox::Back);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		state.set(Xbox::LeftStick);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		state.set(Xbox::RightStick);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		state.set(Xbox::LeftBumper);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		state.set(Xbox::RightBumper);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_A)
	{
		state.set(Xbox::A);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_B)
	{
		state.set(Xbox::B);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_X)
	{
		state.set(Xbox::X);
	}

	if (aXInputButtons & XINPUT_GAMEPAD_Y)
	{
		state.set(Xbox::Y);
	}

	return state;
}
