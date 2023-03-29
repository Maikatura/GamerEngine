#pragma once
#include "imgui/imgui_internal.h"
#include "Renderer/Model/Entity.h"


class DropHandler
{
public:
	static std::wstring DropFileScene(ImRect dropRect = { 0,0,0,0 }, ImGuiID aId = 0);

	static std::wstring DropFileEntity(Entity& aEntity);
};
