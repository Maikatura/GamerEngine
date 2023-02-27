#include "stdafx.h"
#include "GraphNode_GetGameObjectPosition.h"
#include "../../GraphNodeInstance.h"
#include "../../GameWorld.h"
#include "tga2d/sprite/sprite.h"

GraphNode_GetGameObjectPosition::GraphNode_GetGameObjectPosition()
{
	CreatePin("Object", PinDirection::PinDirection_IN, DataType::Int);
	CreatePin("X", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("Y", PinDirection::PinDirection_OUT, DataType::Float);
	CreatePin("Z", PinDirection::PinDirection_OUT, DataType::Float);

}

int GraphNode_GetGameObjectPosition::OnExec(GraphNodeInstance* aNodeInstance)
{
	auto gameObjects = CGameWorld::GetGameObjects();

	const int objectIndex = GetPinData<int>(aNodeInstance, 0);

	if(objectIndex >= gameObjects.size() || objectIndex < 0)
	{
		return -1;
	}
	RECT clientRect;
	GetClientRect(*Tga2D::CEngine::GetInstance()->GetHWND(), &clientRect);

	float xSize = clientRect.left - clientRect.right;
	float ySize = clientRect.bottom - clientRect.top;


	const float x = gameObjects[objectIndex]->GetPosition().x;
	const float y = gameObjects[objectIndex]->GetPosition().y;
	const float z = 0.0f;

	SetPinData(aNodeInstance, 1, x);
	SetPinData(aNodeInstance, 2, y);
	SetPinData(aNodeInstance, 3, z);

	return -1; // If the node doesn't have a flow, return -1 see the print node for reference
}
