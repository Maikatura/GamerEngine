#pragma once
#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"
#include "Utilites/KeyCodes.h"
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

	static bool IsKeyReleased(CommonUtilities::Key::Code aKey);
	static bool IsKeyDown(CommonUtilities::Key::Code aKey);
	static bool IsKeyPressed(CommonUtilities::Key::Code aKey);

	static float GetMouseWheel();

	static bool IsMouseDown(CommonUtilities::Mouse::Button aMouseKey);
	static bool IsMousePressed(CommonUtilities::Mouse::Button aMouseKey);
	static bool IsMouseReleased(CommonUtilities::Mouse::Button aMouseKey);

	static Vector2f GetMouseDelta();
	static Vector2i GetMousePos();

	static void SetMousePos(Vector2i aPos);

	static void LockMouse(CommonUtilities::CursorLockMode aLock);

private:
	inline static Ref<CommonUtilities::KeyboardInput> myKeyboard;
	inline static Ref<CommonUtilities::MouseInput> myMouseButton;
	inline static Ref<CommonUtilities::MouseCursor> myMouseMovement;
};