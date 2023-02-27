#include <GraphicsEngine.pch.h>
#include <Renderer/Input/Input.h>
#include <Utilites/InputManager.h>

void Input::Init()
{
	myInput = std::make_shared<CommonUtilities::InputManager>();
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

CommonUtilities::Vector2<float> Input::GetMouseDelta()
{
	return { static_cast<float>(myInput->MouseDelta().x), static_cast<float>(myInput->MouseDelta().y) };
}

CommonUtilities::Vector2<float> Input::GetMousePos()
{
	return { static_cast<float>(myInput->GetMousePos().x), static_cast<float>(myInput->GetMousePos().y) };
}
