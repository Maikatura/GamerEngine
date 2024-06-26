#include "pch.h"
#include "MouseCursor.h"

#include <cassert>

#include "Windows.h"
#include "ShellScalingApi.h"
#include <hidusage.h>
#include <windowsx.h>

#include "Utilites/WindowsUtils.h"

CommonUtilities::MouseCursor::MouseCursor(HWND aWindowHandle)
	: myOwnerWindowHandle(aWindowHandle)
{
	RAWINPUTDEVICE RID[1];
	RID[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	RID[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	RID[0].dwFlags = RIDEV_INPUTSINK;
	RID[0].hwndTarget = myOwnerWindowHandle;

	if (!RegisterRawInputDevices(&RID[0], 1, sizeof(RID[0])))
	{
		//assert(false, "Failed to register raw input device!");
	}
}

void CommonUtilities::MouseCursor::Update()
{
	//assert(myOwnerWindowHandle, "Window Handle is nullptr.");

	if (myIsVisible)
	{
		while (ShowCursor(true) < 0)
		{
		}
	}
	else
	{
		while (ShowCursor(false) >= 0)
		{
		}
	}

	if (myOwnerWindowHandle == GetFocus())
	{
			
			/*POINT newCursorPosition;
			GetCursorPos(&newCursorPosition);
			ScreenToClient(myOwnerWindowHandle, &newCursorPosition);
			myPosition = { newCursorPosition.x, newCursorPosition.y };*/

			

			myRawMouseDelta = myNextMouseDelta - myOldMouseDelta;
			myOldMouseDelta = myNextMouseDelta;


			switch (myCursorLockMode)
			{
				case CursorLockMode::None:
					myPositionDelta = myPosition - myPreviousPosition;
					myPreviousPosition = myPosition;
					ClipCursor(NULL);
					break;

				case CursorLockMode::Locked:
				{
					myPositionDelta = myPosition - myPreviousPosition;

					POINT previousPosition { myPreviousPosition.x, myPreviousPosition.y };
					ClientToScreen(myOwnerWindowHandle, &previousPosition);
					SetCursorPos(previousPosition.x, previousPosition.y);
					myPosition = myPreviousPosition;

					//For when raw delta is properly setup x.x
					/*RECT windowRect;
					GetWindowRect(myOwnerWindowHandle, &windowRect);
					POINT pos;
					pos.x = windowRect.left + (windowRect.right - windowRect.left) / 2;
					pos.y = windowRect.top + (windowRect.bottom - windowRect.top) / 2;
					SetCursorPos(pos.x, pos.y);
					myPosition = { pos.x, pos.y };*/
					break;
				}

				case CursorLockMode::LockedToWindow:
				{
					RECT clientRect;
					GetClientRect(myOwnerWindowHandle, &clientRect);
					ClientRectToScreenRect(myOwnerWindowHandle, clientRect);
					ClipCursor(&clientRect);
					break;
				}
			}
	}
	else
	{
		myRawMouseDelta = { 0,0 };
		myPositionDelta = { 0, 0 };
	}
}

bool CommonUtilities::MouseCursor::ProcessEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam)
{

	bool returnTrue = false;


	if(aMessage == WM_MOUSEMOVE)
	{
		myPosition.x = static_cast<int>(GET_X_LPARAM(aLParam));
		myPosition.y = static_cast<int>(GET_Y_LPARAM(aLParam));

		returnTrue = true;
	}

	if(aMessage == WM_INPUT)
	{
		myRawBuffer.clear();

		UINT size;
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(aLParam),RID_INPUT,nullptr, &size,sizeof(RAWINPUTHEADER)) == -1)
		{
			return false;
		}

		myRawBuffer.resize(size);

		if(GetRawInputData(reinterpret_cast<HRAWINPUT>(aLParam),RID_INPUT,myRawBuffer.data(),&size,sizeof(RAWINPUTHEADER)) != size)
		{
			return false;
		}

		auto& ri = reinterpret_cast<const RAWINPUT&>(*myRawBuffer.data());
		auto& rawMouse = ri.data.mouse;
		if(ri.header.dwType == RIM_TYPEMOUSE && (rawMouse.lLastX != 0))
		{
			myNextMouseDelta.x += static_cast<float>(rawMouse.lLastX);
		}

		if(ri.header.dwType == RIM_TYPEMOUSE && (rawMouse.lLastY != 0))
		{
			myNextMouseDelta.y += static_cast<float>(rawMouse.lLastY);
		}


		returnTrue = true;
	}



	return returnTrue;
}

bool CommonUtilities::MouseCursor::HasMovedSinceLastUpdate() const
{
	return myRawMouseDelta.x != 0 || myRawMouseDelta.y != 0;
}