#include <GraphicsEngine.pch.h>
#include <Input/Input.h>
#include <Utilites/InputManager.h>

#include "Utilites/Pointers.h"

void Input::Init(HWND aHWND)
{
	myInput = MakeRef<CommonUtilities::InputManager>(aHWND);
}

void Input::Update()
{
	myInput->Update();
}

void Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	myInput->UpdateEvents(message, wParam, lParam);
}

bool Input::IsMouseDown(const int aMouseKey)
{
	return myInput->IsMouseDown(aMouseKey);
}

bool Input::IsMousePressed(const int aMouseKey)
{
	return myInput->IsMousePressed(aMouseKey);
}

bool Input::IsMouseReleased(const int aMouseKey)
{
	return myInput->IsMouseReleased(aMouseKey);
}

bool Input::IsKeyReleased(const int aKey)
{
	return myInput->IsKeyReleased(aKey);
}

bool Input::IsKeyDown(const int aKey)
{
	return myInput->IsKeyDown(aKey);
}

bool Input::IsKeyPressed(const int aKey)
{
	return myInput->IsKeyPressed(aKey);
}

float Input::GetMouseWheel()
{
	return myInput->ScrollDelta();
}

CommonUtilities::Vector2<float> Input::GetMouseDelta()
{
	return { static_cast<float>(myInput->MouseDelta().x), static_cast<float>(myInput->MouseDelta().y) };
}

Vector2i Input::GetMousePos()
{
	return myInput->GetMousePos();
}

void Input::SetMousePos(Vector2i aPos)
{



	myInput->SetMousePos(aPos);
}
