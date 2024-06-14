#include <GraphicsEngine.pch.h>
#include <Input/Input.h>
#include "Utilites/Pointers.h"

void Input::Init(HWND aHWND)
{
	myKeyboard = MakeRef<CommonUtilities::KeyboardInput>();
	myMouseButton = MakeRef<CommonUtilities::MouseInput>(aHWND);
	myMouseMovement = MakeRef<CommonUtilities::MouseCursor>(aHWND);
}

void Input::Update()
{
	myKeyboard->Update();
	myMouseButton->Update();
	myMouseMovement->Update();
}

void Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	myKeyboard->ProcessEvents(message, wParam, lParam);
	myMouseButton->ProcessEvents(message, wParam, lParam);
	myMouseMovement->ProcessEvents(message, wParam, lParam);
}

bool Input::IsMouseDown(CommonUtilities::Mouse::Button aMouseKey)
{
	return myMouseButton->IsMousePressed(aMouseKey); // myInput->IsKeyPressed(aMouseKey);
}

bool Input::IsMousePressed(CommonUtilities::Mouse::Button aMouseKey)
{
	return myMouseButton->WasMousePressedThisFrame(aMouseKey);//myInput->IsMousePressed(aMouseKey);
}

bool Input::IsMouseReleased(CommonUtilities::Mouse::Button aMouseKey)
{
	return myMouseButton->IsMouseReleased(aMouseKey); //myInput->IsMouseReleased(aMouseKey);
}

bool Input::IsKeyReleased(CommonUtilities::Key::Code aKey)
{
	return myKeyboard->IsKeyReleased(aKey);
}

bool Input::IsKeyDown(CommonUtilities::Key::Code aKey)
{
	return myKeyboard->IsKeyPressed(aKey); // myInput->is(aKey);
}

bool Input::IsKeyPressed(CommonUtilities::Key::Code aKey)
{
	return myKeyboard->WasKeyPressedThisFrame(aKey);
}

float Input::GetMouseWheel()
{
	return 0.0f; // myInput->ScrollDelta();
}

CommonUtilities::Vector2<float> Input::GetMouseDelta()
{
	return { static_cast<float>(myMouseMovement->GetRawMouseDelta().x), static_cast<float>(myMouseMovement->GetRawMouseDelta().y) };
}

Vector2i Input::GetMousePos()
{
	return myMouseMovement->GetPosition();
}

void Input::SetMousePos(Vector2i aPos)
{



	//myInput->SetMousePos(aPos);
}

void Input::LockMouse(int aLock)
{

	//myInput->LockMouse(aLock);
}
