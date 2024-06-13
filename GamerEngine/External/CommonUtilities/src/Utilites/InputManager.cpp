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
	return (myCurrentMousePosition.x != myPreviousActiveMousePosition.x && myCurrentMousePosition.y != myPreviousActiveMousePosition.y);
}

Vector2i CommonUtilities::InputManager::MouseDelta()
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


	//OnMouseMove(currentMousePos.x, currentMousePos.y);

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
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

	switch (myMouseLockState)
	{
		case CommonUtilities::Mouse::Lock_Center:
		{
			RECT rect;
			GetClientRect(myHWND, &rect);
			POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
			ClientToScreen(myHWND, &center);
			SetCursorPos(center.x, center.y);
			break;
		}
		case CommonUtilities::Mouse::Lock_WithinWindow:
		{
			RECT rect;
			GetClientRect(myHWND, &rect);
			POINT topLeft = { rect.left, rect.top };
			POINT bottomRight = { rect.right, rect.bottom };
			ClientToScreen(myHWND, &topLeft);
			ClientToScreen(myHWND, &bottomRight);
			rect.left = topLeft.x;
			rect.top = topLeft.y;
			rect.right = bottomRight.x;
			rect.bottom = bottomRight.y;
			ClipCursor(&rect);
			break;
		}
	}

	

	myOldMouseLockState = myMouseLockState;
}

void CommonUtilities::InputManager::OnMouseMove(int xPos, int yPos)
{
	Vector2i currentMousePos;
	currentMousePos.x = xPos;
	currentMousePos.y = yPos;

	// Calculate the 
	myMouseDelta.x = static_cast<int>(currentMousePos.x) - myPreviousActiveMousePosition.x;
	myMouseDelta.y = static_cast<int>(currentMousePos.y) - myPreviousActiveMousePosition.y;

	// Optionally, lock the mouse to the center of the window
	if (myMouseLockState == CommonUtilities::Mouse::Lock_Center) 
	{
		RECT rect;
		GetClientRect(myHWND, &rect);
		POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
		ClientToScreen(myHWND, &center);
		SetCursorPos(center.x, center.y);

		// Update last mouse position to the center
		myPreviousActiveMousePosition.x = center.x;
		myPreviousActiveMousePosition.y = center.y;

		POINT p;
		p.x = myPreviousActiveMousePosition.x;
		p.y = myPreviousActiveMousePosition.y;
		ScreenToClient(myHWND, &p);
	}
	else 
	{

		if (currentMousePos == myPreviousActiveMousePosition)
		{
			myMouseDelta.x = 0;
			myMouseDelta.y = 0;

		}

		// Update the last mouse position
		myPreviousActiveMousePosition.x = currentMousePos.x;
		myPreviousActiveMousePosition.y = currentMousePos.y;

		
	}
}

