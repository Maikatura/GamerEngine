#include "stdafx.h"
#include "GraphNode_SetGameObjectScale.h"
#include "../../GraphNodeInstance.h"
#include "../../GameWorld.h"
#include "tga2d/sprite/sprite.h"

GraphNode_SetGameObjectScale::GraphNode_SetGameObjectScale()
{
	CreatePin("", PinDirection::PinDirection_IN);
	CreatePin("", PinDirection::PinDirection_OUT);

	CreatePin("Object", PinDirection::PinDirection_IN, DataType::Int);
	CreatePin("X", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Y", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Z", PinDirection::PinDirection_IN, DataType::Float);
}

int GraphNode_SetGameObjectScale::OnExec(GraphNodeInstance* aNodeInstance)
{
	auto gameObjects = CGameWorld::GetGameObjects();

	const int objectIndex = GetPinData<int>(aNodeInstance, 2);
	if(objectIndex >= gameObjects.size() || objectIndex < 0)
	{
		return -1;
	}
	const float x = GetPinData<float>(aNodeInstance, 3);
	const float y = GetPinData<float>(aNodeInstance, 4);
	const float z = GetPinData<float>(aNodeInstance, 5);

	gameObjects[objectIndex]->SetSizeRelativeToImage({ x,y });

	return -1; // If the node doesn't have a flow, return -1 see the print node for reference
}