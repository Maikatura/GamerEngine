#pragma once
#include <InputManager.h>

class Input
{
public:
	static void Update()
	{
		myInput.Update();
	}

	static void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		myInput.UpdateEvents(message, wParam, lParam);
	}

	static bool IsMouseDown(const int aMouseKey)
	{
		return myInput.IsMouseDown(aMouseKey);
	}


	static bool IsKeyReleased(const int aKey)
	{
		return myInput.IsKeyReleased(aKey);
	}

	static bool IsKeyDown(const int aKey)
	{
		return myInput.IsKeyDown(aKey);
	}

	static bool IsKeyPressed(const int aKey)
	{
		return myInput.IsKeyPressed(aKey);
	}

	static Vector2f GetMouseDelta()
	{
		return { static_cast<float>(myInput.MouseDelta().x), static_cast<float>(myInput.MouseDelta().y) };
	}

	static Vector2f GetMousePos()
	{
		return { static_cast<float>(myInput.GetMousePos().x), static_cast<float>(myInput.GetMousePos().y) };
	}

private:
	inline static CommonUtilities::InputManager myInput;
};