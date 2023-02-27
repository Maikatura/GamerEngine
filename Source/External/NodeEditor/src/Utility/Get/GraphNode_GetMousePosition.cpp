#include "stdafx.h"
#include "GraphNode_GetMousePosition.h"
#include "../../GraphNodeInstance.h"
#include "../../Game.h"
#include "Math/Vector2.hpp"

GraphNode_GetMousePosition::GraphNode_GetMousePosition()
{
	CreatePin("X", PinDirection::PinDirection_OUT, DataType::Int);
	CreatePin("Y", PinDirection::PinDirection_OUT, DataType::Int);
}

int GraphNode_GetMousePosition::OnExec(GraphNodeInstance* aNodeInstance)
{
	auto mousePos = CGame::GetInput().GetMousePosRelativeToWindowOneScale();
	
	CommonUtilities::Vector2<float> pos{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	SetPinData(aNodeInstance, 0, pos.x);
	SetPinData(aNodeInstance, 1, pos.y);

	return -1;
}
