#include "pch.h"
#include "KeyboardInput.h"
#include <Utilites/InputUtil.h>

namespace CommonUtilities
{

	bool KeyboardInput::IsKeyPressed(Key::Code aKeyCode) const
	{
		return myCurrentKeyboardState[aKeyCode];
	}

	bool KeyboardInput::WasKeyPressedThisFrame(Key::Code aKeyCode) const
	{
		return myCurrentKeyboardState[aKeyCode] && !myPreviousKeyboardState[aKeyCode];
	}

	bool KeyboardInput::IsKeyReleased(Key::Code aKeyCode) const
	{
		return !myCurrentKeyboardState[aKeyCode];
	}

	bool KeyboardInput::WasKeyReleasedThisFrame(Key::Code aKeyCode) const
	{
		return !myCurrentKeyboardState[aKeyCode] && myPreviousKeyboardState[aKeyCode];
	}

	bool KeyboardInput::ProcessEvents(uint32_t aMessage, unsigned long long aWParam, long long aLParam)
	{
		switch (aMessage)
		{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				myNextKeyboardState.set(WindowsVKToCUKey(static_cast<int>(aWParam), HIWORD(aLParam)));
				return true;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				myNextKeyboardState.reset(WindowsVKToCUKey(static_cast<int>(aWParam), HIWORD(aLParam)));
				return true;
			}
		}
		return false;
	}

	void KeyboardInput::Update()
	{
		myPreviousKeyboardState = myCurrentKeyboardState;
		myCurrentKeyboardState = myNextKeyboardState;
	}

	void KeyboardInput::ResetInput()
	{
		myNextKeyboardState.reset();
		myPreviousKeyboardState.reset();
		myCurrentKeyboardState.reset();
	}
}
