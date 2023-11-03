#pragma once
#include "Utilites/Pointers.h"


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

	static bool IsMouseDown(const int aMouseKey);
	static CommonUtilities::Vector2<float> GetMouseDelta();
	static  CommonUtilities::Vector2<float> GetMousePos();

private:
	inline static Ref<CommonUtilities::InputManager> myInput;
};