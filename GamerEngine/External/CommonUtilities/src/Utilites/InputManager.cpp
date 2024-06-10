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
	return (myCurrentActiveMousePosition.x != myPreviousActiveMousePosition.x && myCurrentActiveMousePosition.y != myPreviousActiveMousePosition.y);
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
	return POINT{ static_cast<long>(myCurrentActiveMousePosition.x) , static_cast<long>(myCurrentActiveMousePosition.y) };
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


	//POINT currentMousePos = {};
	//::GetCursorPos(&currentMousePos);

	OnMouseMove(myCurrentMousePosition.x, myCurrentMousePosition.y);

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
		POINT windowPosition;
		RECT windowRect;
		GetWindowRect(myHWND, &windowRect);

		windowPosition.x = windowRect.left;
		windowPosition.y = windowRect.top;

		//OnMouseMove(currentMousePos.x, currentMousePos.y);
		myCurrentMousePosition.x = GET_X_LPARAM(lParam) + windowPosition.x;
		myCurrentMousePosition.y = GET_Y_LPARAM(lParam) + windowPosition.y;
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


	// Set the cursor position to the middle of the window
	//SetCursorPos(aPos.x, aPos.y);

	int windowX = windowRect.left;
	int windowY = windowRect.top;
	//
	//// Calculate the new cursor position
	int newX = windowX + aPos.x;
	int newY = windowY + aPos.y;
	// Set the cursor position
	SetCursorPos(newX, newY);
	mySettingPosition = true;
	
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

void CommonUtilities::InputManager::LockMouse(CommonUtilities::MouseLock aLockState)
{
	myMouseLockState = aLockState;
}

void CommonUtilities::InputManager::OnMouseMove(int xPos, int yPos)
{
	myCurrentActiveMousePosition.x = xPos;
	myCurrentActiveMousePosition.y = yPos;

	

	if (myFirstTime)
	{
		myPreviousActiveMousePosition.x = myCurrentActiveMousePosition.x;
		myPreviousActiveMousePosition.y = myCurrentActiveMousePosition.y;
		myFirstTime = false;
	}

	if ((myCurrentMousePosition.x != myPreviousActiveMousePosition.x || myCurrentMousePosition.y != myPreviousActiveMousePosition.y))
	{

		switch (myMouseLockState)
		{
			case CommonUtilities::Mouse::Lock_Center:
			{


					

				// Calculate the middle of the window
				RECT rect;
				GetWindowRect(myHWND, &rect);
				int middleX = rect.left + (rect.right - rect.left) / 2;
				int middleY = rect.top + (rect.bottom - rect.top) / 2;


				int middleMouseX = middleX - myCurrentActiveMousePosition.x;
				int middleMosueY = middleY - myCurrentActiveMousePosition.y;

				myCurrentActiveMousePosition.x = middleMouseX;
				myCurrentActiveMousePosition.y = middleMosueY;

				myMouseDelta.x = static_cast<float>(myCurrentActiveMousePosition.x) - static_cast<float>(myPreviousActiveMousePosition.x);
				myMouseDelta.y = static_cast<float>(myCurrentActiveMousePosition.y) - static_cast<float>(myPreviousActiveMousePosition.y);

				// Set the cursor position to the middle of the window
				//SetCursorPos(middleX, middleY);

				SetMousePos({ middleX,middleY });

				// Store the middle position as the previous position for the next frame
				myPreviousActiveMousePosition.x = static_cast<int>(middleX);
				myPreviousActiveMousePosition.y = static_cast<int>(middleY);
				
			}
			break;
			case CommonUtilities::Mouse::Lock_CurrentPos:
			{

				POINT windowPosition;
				RECT windowRect;
				GetWindowRect(myHWND, &windowRect);

				windowPosition.x = windowRect.left;
				windowPosition.y = windowRect.top;

				// Calculate the active mouse position relative to the window
				int myCurrentActiveMousePositionX = myCurrentMousePosition.x;// - windowPosition.x;
				int myCurrentActiveMousePositionY = myCurrentMousePosition.y;// - windowPosition.y;

				// Calculate the mouse delta
				if (!mySettingPosition)
				{
					myMouseDelta.x = static_cast<float>(myCurrentActiveMousePositionX - myPreviousActiveMousePosition.x);
					myMouseDelta.y = static_cast<float>(myCurrentActiveMousePositionY - myPreviousActiveMousePosition.y);

					SetMousePos({ myCurrentActiveMousePositionX, myCurrentActiveMousePositionY });
				}
				else
				{
					myCurrentActiveMousePosition.x = 0;
					myCurrentActiveMousePosition.y = 0;

					myMouseDelta.x = 0;
					myMouseDelta.y = 0;
					mySettingPosition = false;
				}

				std::cout << "X: " << myCurrentActiveMousePositionX << " Y: " << myCurrentActiveMousePositionY << std::endl;


				// Set the cursor position back to the saved position (relative to the window)
				//SetMousePos({ mySavedMousePosition.x,mySavedMousePosition.y });
				// Store the current active position as the previous position for the next frame
				myPreviousActiveMousePosition.x = myCurrentActiveMousePositionX;
				myPreviousActiveMousePosition.y = myCurrentActiveMousePositionY;
				
				break;
				
				
			}
			break;
			case CommonUtilities::Mouse::None:
			{
				myMouseDelta.x = static_cast<float>(myCurrentActiveMousePosition.x) - static_cast<float>(myPreviousActiveMousePosition.x);
				myMouseDelta.y = static_cast<float>(myCurrentActiveMousePosition.y) - static_cast<float>(myPreviousActiveMousePosition.y);
				myPreviousActiveMousePosition = myCurrentActiveMousePosition;
				
			}
			break;
		}
	}
	else
	{
		
		myMouseDelta.x = 0;
		myMouseDelta.y = 0;
	}

	if ((myMouseLockState == CommonUtilities::Mouse::Lock_CurrentPos && myOldMouseLockState != CommonUtilities::Mouse::Lock_CurrentPos))
	{
		POINT windowPosition;
		RECT windowRect;
		GetWindowRect(myHWND, &windowRect);

		windowPosition.x = windowRect.left;
		windowPosition.y = windowRect.top;

		//OnMouseMove(currentMousePos.x, currentMousePos.y);
		mySavedMousePosition.x = myCurrentMousePosition.x + windowPosition.x;
		mySavedMousePosition.y = myCurrentMousePosition.y + windowPosition.y;

		std::cout << "Lock Current Pos" << std::endl;
	}

	if ((myMouseLockState == CommonUtilities::Mouse::None && myOldMouseLockState == CommonUtilities::Mouse::Lock_CurrentPos))
	{
		SetMousePos({ mySavedMousePosition.x,mySavedMousePosition.y });
		std::cout << "Lock none" << std::endl;
	}


	myOldMouseLockState = myMouseLockState;



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

