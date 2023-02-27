#include "Editor.pch.h"
#include <Input/InputInternal.h>
#include <ImGui/imgui.h>

#define CheckIfPresent(aReturnValueIfImGuiNotPresent) if(!ImGui::GetCurrentContext()) return aReturnValueIfImGuiNotPresent

bool InputInternal::IsMousePressed(const int aMouseKey)
{
	CheckIfPresent(false);
	return ImGui::IsMouseClicked(aMouseKey);
}

bool InputInternal::IsMouseDown(const int aMouseKey)
{
	CheckIfPresent(false);
	return ImGui::IsMouseDown(aMouseKey);
}

bool InputInternal::IsMouseReleased(const int aMouseKey)
{
	CheckIfPresent(false);
	return ImGui::IsMouseReleased(aMouseKey);
}

Vector2f InputInternal::GetMouseDelta()
{
	CheckIfPresent(Vector2f(0, 0));

	ImGuiIO& io = ImGui::GetIO();
	return { io.MouseDelta.x,io.MouseDelta.y };
}

Vector2f InputInternal::GetMousePos()
{
	CheckIfPresent(Vector2f(0, 0));
	return { ImGui::GetMousePos().x,ImGui::GetMousePos().y };
}

bool InputInternal::IsKeyReleased(const int aKey)
{
	CheckIfPresent(false);
	return ImGui::IsKeyReleased((ImGuiKey)aKey);
}

bool InputInternal::IsKeyDown(const int aKey)
{
	CheckIfPresent(false);
	return ImGui::IsKeyDown((ImGuiKey)aKey);
}

bool InputInternal::IsKeyPressed(const int aKey)
{
	CheckIfPresent(false);
	return ImGui::IsKeyPressed((ImGuiKey)aKey);
}
