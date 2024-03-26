#pragma once
#include "Utilites/Pointers.h"
#include <windows.h>

namespace CommonUtilities
{
	class InputManager;

	template<class T>
	class Vector2;
}
class Input
{
public:
	static void Init();
	static void Update();
	static void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	static bool IsKeyReleased(const int aKey);
	static bool IsKeyDown(const int aKey);
	static bool IsKeyPressed(const int aKey);

	static float GetMouseWheel();

	static bool IsMouseDown(const int aMouseKey);
	static bool IsMousePressed(const int aMouseKey);
	static bool IsMouseReleased(const int aMouseKey);

	static CommonUtilities::Vector2<float> GetMouseDelta();
	static Vector2i GetMousePos();

	static void SetMousePos(int aX, int aY);

private:
	inline static Ref<CommonUtilities::InputManager> myInput;
};