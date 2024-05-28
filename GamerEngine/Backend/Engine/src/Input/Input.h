#pragma once
#include "Utilites/Pointers.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace CommonUtilities
{
	class InputManager;

	template<class T>
	class Vector2;
}
class Input
{
public:
	static void Init(HWND aHWND);
	static void Update();
	static void UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	static bool IsKeyReleased(const int aKey);
	static bool IsKeyDown(const int aKey);
	static bool IsKeyPressed(const int aKey);

	static float GetMouseWheel();

	static bool IsMouseDown(const int aMouseKey);
	static bool IsMousePressed(const int aMouseKey);
	static bool IsMouseReleased(const int aMouseKey);

	static Vector2f GetMouseDelta();
	static Vector2i GetMousePos();

	static void SetMousePos(Vector2i aPos);

	static void LockMouse(int aLock);

private:
	inline static Ref<CommonUtilities::InputManager> myInput;
};