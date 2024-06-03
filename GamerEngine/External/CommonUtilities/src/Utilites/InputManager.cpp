#include "pch.h"
#include <Utilites/InputManager.h>
#include <string>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windowsx.h>


CommonUtilities::InputManager::InputManager(HWND aHWND) : myCurrentScrollDelta(0), mySavedScrollDelta(0), myLastScrollDelta(0), myHWND(aHWND)
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
	if (myCurrentMousePress.size() <= aMouseKeyCode)
	{
		return false;
	}

	return(myCurrentMousePress[aMouseKeyCode] && myPreviousMousePress[aMouseKeyCode]);
}


bool CommonUtilities::InputManager::IsMousePressed(const int aMouseKeyCode) const
{
	if (myCurrentMousePress.size() <= aMouseKeyCode)
	{
		return false;
	}

	return (myCurrentMousePress[aMouseKeyCode] && !myPreviousMousePress[aMouseKeyCode]);
}

bool CommonUtilities::InputManager::IsMouseReleased(const int aMouseKeyCode) const
{
	if (myCurrentMousePress.size() <= aMouseKeyCode)
	{
		return false;
	}

	return (!myCurrentMousePress[aMouseKeyCode] && myPreviousMousePress[aMouseKeyCode]);
}


bool CommonUtilities::InputManager::IsMouseMoving() const
{
	return (myCurrentMousePosition.x != myPreviousMousePosition.x && myCurrentMousePosition.y != myPreviousMousePosition.y);
}


Vector2f CommonUtilities::InputManager::MouseDelta()
{
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
	return POINT{ static_cast<long>(myCurrentMousePosition.x) , static_cast<long>(myCurrentMousePosition.y) };
}

POINT CommonUtilities::InputManager::GetMousePosRelativeToWindow() const
{
	return POINT{ static_cast<long>(myCurrentRelativeMousePosition.x) , static_cast<long>(myCurrentRelativeMousePosition.y) };
}

void CommonUtilities::InputManager::Update()
{
	//if (GetFocus() != myHWND)
	//{
	//	// Reset all key states
	//	for (int i = 0; i < 256; i++)
	//	{
	//		mySavedWindowsKeyboardState[i] = false;
	//	}

	//	// Reset all mouse states
	//	for (int i = 0; i < 7; i++) // Increase the size to 7
	//	{
	//		mySavedMousePress[i] = false;
	//	}
	//}


	POINT currentMousePos = {};
	::GetCursorPos(&currentMousePos);

	OnMouseMove(currentMousePos.x, currentMousePos.y);

	myPreviousKeyboardState = myCurrentKeyboardState;
	myCurrentKeyboardState = mySavedWindowsKeyboardState;

	

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
		//OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

void CommonUtilities::InputManager::SetMousePos(Vector2i aPos)
{
	// Get the window's position and size
	RECT windowRect;
	GetWindowRect(myHWND, &windowRect);



	// Calculate the middle of the window
	int middleX = windowRect.left + (windowRect.right - windowRect.left) / 2;
	int middleY = windowRect.top + (windowRect.bottom - windowRect.top) / 2;

	// Set the cursor position to the middle of the window
	SetCursorPos(middleX, middleY);

	//int windowX = windowRect.left;
	//int windowY = windowRect.top;
	//
	//// Calculate the new cursor position
	//int newX = windowX + aPos.x;
	//int newY = windowY + aPos.y;
	//// Set the cursor position
	//SetCursorPos(newX, newY);
}

Vector2i CommonUtilities::InputManager::GetMousePos()
{
	Vector2i mousePos;
	POINT p;
	GetCursorPos(&p);

	// Get the window's position
	RECT windowRect;
	GetWindowRect(myHWND, &windowRect);
	int windowX = windowRect.left;
	int windowY = windowRect.top;

	// Calculate the cursor position relative to the window
	mousePos.x = p.x - windowX;
	mousePos.y = p.y - windowY;

	return mousePos;
}

void CommonUtilities::InputManager::LockMouse(int aLockState)
{
	myMouseLockState = aLockState;

	if (myMouseLockState == CommonUtilities::Mouse::Lock_CurrentPos && myOldMouseLockState != CommonUtilities::Mouse::Lock_CurrentPos)
	{
		POINT currentMousePos = {};
		::GetCursorPos(&currentMousePos);


		//OnMouseMove(currentMousePos.x, currentMousePos.y);
		mySavedMousePosition.x = currentMousePos.x;
		mySavedMousePosition.y = currentMousePos.y;
	}

	if (myMouseLockState == CommonUtilities::Mouse::None && myOldMouseLockState == CommonUtilities::Mouse::Lock_CurrentPos)
	{
		SetCursorPos(mySavedMousePosition.x, mySavedMousePosition.y);
	}

	myOldMouseLockState = myMouseLockState;
}

void CommonUtilities::InputManager::OnMouseMove(int xPos, int yPos)
{
	myCurrentMousePosition.x = xPos;
	myCurrentMousePosition.y = yPos;

	if (myFirstTime)
	{
		myPreviousMousePosition.x = myCurrentMousePosition.x;
		myPreviousMousePosition.y = myCurrentMousePosition.y;
		myFirstTime = false;
	}

	if (myCurrentMousePosition.x != myPreviousMousePosition.x || myCurrentMousePosition.y != myPreviousMousePosition.y)
	{
		myMouseDelta.x = static_cast<float>(myCurrentMousePosition.x) - static_cast<float>(myPreviousMousePosition.x);
		myMouseDelta.y = static_cast<float>(myCurrentMousePosition.y) - static_cast<float>(myPreviousMousePosition.y);


		switch (myMouseLockState)
		{
			case  CommonUtilities::Mouse::Lock_Center:
			{
				// Calculate the middle of the window
				RECT rect;
				GetWindowRect(myHWND, &rect);
				int middleX = rect.left + (rect.right - rect.left) / 2;
				int middleY = rect.top + (rect.bottom - rect.top) / 2;

				// Set the cursor position to the middle of the window
				SetCursorPos(middleX, middleY);

				// Store the middle position as the previous position for the next frame
				myPreviousMousePosition.x = static_cast<int>(middleX);
				myPreviousMousePosition.y = static_cast<int>(middleY);
				break;
			}
			case  CommonUtilities::Mouse::Lock_CurrentPos:
			{

				// Change to a clipping size instead.
				SetCursorPos(mySavedMousePosition.x, mySavedMousePosition.y);

				// Store the current position as the previous position for the next frame
				myPreviousMousePosition.x = mySavedMousePosition.x;
				myPreviousMousePosition.y = mySavedMousePosition.y;
				
				break;
			}
			case  CommonUtilities::Mouse::None:
			{
				myPreviousMousePosition = myCurrentMousePosition;
				break;
			}
		}
	}
	else
	{
		myMouseDelta.x = 0;
		myMouseDelta.y = 0;
	}


	// Calculate the mouse delta
	//if (xPos != myPreviousMousePosition.x || yPos != myPreviousMousePosition.y)
	//{
	//	// Calculate the mouse delta
	//	myMouseDelta.x = xPos - myPreviousMousePosition.x;
	//	myMouseDelta.y = yPos - myPreviousMousePosition.y;

	//	// Use deltaX and deltaY for your game's input handling
	//}
	//else
	//{
	//	// If the mouse hasn't moved, reset the mouse delta
	//	myMouseDelta.x = 0;
	//	myMouseDelta.y = 0;
	//}

	//// Store the new position for the next frame
	//myPreviousMousePosition.x = static_cast<float>(xPos);
	//myPreviousMousePosition.y = static_cast<float>(yPos);


	// Use deltaX and deltaY for your game's input handling
	

}

