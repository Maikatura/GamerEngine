#include "pch.h"
#include "MouseInput.h"
#include "Utilites/InputUtil.h"
#include <hidusage.h>

CommonUtilities::MouseInput::MouseInput(HWND aWindowHandle)
	: myOwnerWindowHandle(aWindowHandle)
{

}

bool CommonUtilities::MouseInput::IsMousePressed(Mouse::Button aMouseButton) const
{
	return myCurrentMouseState[aMouseButton];
}

bool CommonUtilities::MouseInput::WasMousePressedThisFrame(Mouse::Button aMouseButton) const
{
	return myCurrentMouseState[aMouseButton] && !myPreviousMouseState[aMouseButton];
}

bool CommonUtilities::MouseInput::IsMouseReleased(Mouse::Button aMouseButton) const
{
	return !myCurrentMouseState[aMouseButton];
}

bool CommonUtilities::MouseInput::WasMouseReleasedThisFrame(Mouse::Button aMouseButton) const
{
	return !myCurrentMouseState[aMouseButton] && myPreviousMouseState[aMouseButton];
}

bool CommonUtilities::MouseInput::ProcessEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam)
{
	switch (aMessage)
	{
		case WM_LBUTTONDOWN:
			myNextMouseState.set(Mouse::LeftButton);
			return true;

		case WM_LBUTTONUP:
			myNextMouseState.reset(Mouse::LeftButton);
			return true;

		case WM_MBUTTONDOWN:
			myNextMouseState.set(Mouse::MiddleButton);
			return true;

		case WM_MBUTTONUP:
			myNextMouseState.reset(Mouse::MiddleButton);
			return true;

		case WM_RBUTTONDOWN:
			myNextMouseState.set(Mouse::RightButton);
			return true;

		case WM_RBUTTONUP:
			myNextMouseState.reset(Mouse::RightButton);
			return true;

		case WM_XBUTTONDOWN:
			myNextMouseState.set(XButtonFromWParam(aWParam));
			return true;

		case WM_XBUTTONUP:
			myNextMouseState.reset(XButtonFromWParam(aWParam));
			return true;

		case WM_MOUSEWHEEL:
		{
			float wheelDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(aWParam)) / static_cast<float>(WHEEL_DELTA);
			myNextMouseScrollValue.y += wheelDelta;
			return true;
		}

		case WM_MOUSEHWHEEL:
		{
			float wheelDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(aWParam)) / static_cast<float>(WHEEL_DELTA);
			myNextMouseScrollValue.x += wheelDelta;
			return true;
		}
	}

	return false;
}

void CommonUtilities::MouseInput::Update()
{
	myPreviousMouseState = myCurrentMouseState;
	myCurrentMouseState = myNextMouseState;

	myCurrentMouseScrollValue = myNextMouseScrollValue;
	myNextMouseScrollValue = Vector2f{ 0.0f, 0.0f };
}

void CommonUtilities::MouseInput::ResetInput()
{
	myPreviousMouseState.reset();
	myNextMouseState.reset();
	myCurrentMouseState.reset();
	myCurrentMouseScrollValue = Vector2f{ 0.0f, 0.0f };
	myNextMouseScrollValue = Vector2f{ 0.0f, 0.0f };
}
