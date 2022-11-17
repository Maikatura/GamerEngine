#pragma once
#include "imgui_internal.h"
#include "Model/Entity.h"


class DropHandler
{
public:
	static std::wstring DropFileScene(ImRect dropRect = { 0,0,0,0 }, ImGuiID aId = 0);

	static std::wstring DropFileEntity(Entity& aEntity);
};
