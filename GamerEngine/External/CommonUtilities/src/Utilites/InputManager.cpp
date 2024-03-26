#include "pch.h"
#include <Utilites/InputManager.h>
#include <string>
#include <Windowsx.h>


CommonUtilities::InputManager::InputManager() : myCurrentScrollDelta(0), mySavedScrollDelta(0), myLastScrollDelta(0)
{
	
}

CommonUtilities::InputManager::~InputManager()
{
	
}

bool CommonUtilities::InputManager::IsKeyDown(const int aKeyCode) const
{
	return myCurrentKeyboardState[aKeyCode] && myPreviousKeyboardState[aKeyCode];
}

bool CommonUtilities::InputManager::IsKeyUp(const int aKeyCode) const
{
	return !myCurrentKeyboardState[aKeyCode] && !myPreviousKeyboardState[aKeyCode];
}

bool CommonUtilities::InputManager::IsKeyPressed(const int aKeyCode) const
{
	return (myCurrentKeyboardState[aKeyCode] && !myPreviousKeyboardState[aKeyCode]);
}

bool CommonUtilities::InputManager::IsKeyReleased(const int aKeyCode) const
{
	return (!myCurrentKeyboardState[aKeyCode] && myPreviousKeyboardState[aKeyCode]);
}

bool CommonUtilities::InputManager::IsMouseDown(const int aMouseKeyCode) const
{
	return(myCurrentMousePress[aMouseKeyCode] && myPreviousMousePress[aMouseKeyCode]);
}


bool CommonUtilities::InputManager::IsMousePressed(const int aMouseKeyCode) const
{
	return (myCurrentMousePress[aMouseKeyCode] && !myPreviousMousePress[aMouseKeyCode]);
}

bool CommonUtilities::InputManager::IsMouseReleased(const int aMouseKeyCode) const
{
	return (!myCurrentMousePress[aMouseKeyCode] && myPreviousMousePress[aMouseKeyCode]);
}


bool CommonUtilities::InputManager::IsMouseMoving() const
{
	return (myCurrentMousePosition.x != myPrevioustMousePosition.x && myCurrentMousePosition.y != myPrevioustMousePosition.y);
}


Vector2f CommonUtilities::InputManager::MouseDelta()
{
	myMouseDelta.x = myCurrentMousePosition.x - myPrevioustMousePosition.x;
	myMouseDelta.y = myCurrentMousePosition.y - myPrevioustMousePosition.y;

	return myMouseDelta;
}

bool CommonUtilities::InputManager::IsScrolling() const
{
	return myCurrentScrollDelta == myLastScrollDelta;
}

float CommonUtilities::InputManager::ScrollDelta()
{
	float scroll = myCurrentScrollDelta / 120.0f;

	return scroll;
}

POINT CommonUtilities::InputManager::GetMousePos() const
{
	return POINT{static_cast<long>(myCurrentMousePosition.x) , static_cast<long>(myCurrentMousePosition.y)};
}

POINT CommonUtilities::InputManager::GetMousePosRelativeToWindow() const
{
	return POINT{ static_cast<long>(myCurrentRelativeMousePosition.x) , static_cast<long>(myCurrentRelativeMousePosition.y) };
}

void CommonUtilities::InputManager::Update()
{
	myPreviousKeyboardState = myCurrentKeyboardState;
	myCurrentKeyboardState = mySavedWindowsKeyboardState;

	myPrevioustMousePosition = myCurrentMousePosition;
	myCurrentMousePosition = mySavedMousePosition;

	myPreviousMousePress = myCurrentMousePress;
	myCurrentMousePress = mySavedMousePress;

	myLastScrollDelta = myCurrentScrollDelta;
	myCurrentScrollDelta = mySavedScrollDelta;

	mySavedScrollDelta = 0.0f;
}


bool CommonUtilities::InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
		case WM_KEYDOWN:
		{
			mySavedWindowsKeyboardState[wParam] = true;
			return true;
		}
		case WM_KEYUP:
		{
			mySavedWindowsKeyboardState[wParam] = false;
			return true;
		}
		case WM_MOUSEMOVE:
		{
			mySavedMousePosition.x = static_cast<float>(GET_X_LPARAM(lParam));
			mySavedMousePosition.y = static_cast<float>(GET_Y_LPARAM(lParam));
			return true;
		}
		case WM_LBUTTONDOWN:
		{
			mySavedMousePress[0] = true;
			return true;
		}
		case WM_LBUTTONUP:
		{
			mySavedMousePress[0] = false;
			return true;
		}
		case WM_RBUTTONDOWN:
		{
			mySavedMousePress[1] = true;
			return true;
		}
		case WM_RBUTTONUP:
		{
			mySavedMousePress[1] = false;
			return true;
		}
		case WM_MBUTTONDOWN:
		{
			mySavedMousePress[2] = true;
			return true;
		}
		case WM_MBUTTONUP:
		{
			mySavedMousePress[2] = false;
			return true;
		}
		case WM_MOUSEWHEEL:
		{
			mySavedScrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
		}
	}

	return false;
}

void CommonUtilities::InputManager::SetMousePos(int aX, int aY)
{
	SetCursorPos(aX, aY);
}

Vector2i CommonUtilities::InputManager::GetMousePos()
{
	Vector2i mousePos;
	POINT p;
	GetCursorPos(&p);

	mousePos.x = p.x;
	mousePos.y = p.y;

	return mousePos;
}

