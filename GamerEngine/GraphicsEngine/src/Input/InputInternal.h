#pragma once
#include "Math/MathTypes.hpp"

// Only use this class for the editor :)

class InputInternal
{
public:

	static bool IsMousePressed(const int aMouseKey);

	static bool IsMouseDown(const int aMouseKey);

	static bool IsMouseReleased(const int aMouseKey);

	static Vector2f GetMouseDelta();

	static Vector2f GetMousePos();

	static bool IsKeyReleased(const int aKey);

	static bool IsKeyDown(const int aKey);

	static bool IsKeyPressed(const int aKey);

private:
	inline static Vector2f myLastDelta;
};
