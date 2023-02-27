
#include "GraphNode_SetGameObjectPosition.h"
#include "../../GraphNodeInstance.h"


GraphNode_SetGameObjectPosition::GraphNode_SetGameObjectPosition()
{
	CreatePin("", PinDirection::PinDirection_IN);
	CreatePin("", PinDirection::PinDirection_OUT);

	CreatePin("Object", PinDirection::PinDirection_IN, DataType::Int);
	CreatePin("X", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Y", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Z", PinDirection::PinDirection_IN, DataType::Float);
}

int GraphNode_SetGameObjectPosition::OnExec(GraphNodeInstance* aNodeInstance)
{
	//auto gameObjects = CGameWorld::GetGameObjects();

	/*const int objectIndex = GetPinData<int>(aNodeInstance, 2);
	if(objectIndex >= gameObjects.size() || objectIndex < 0)
	{
		return -1;
	}
	const float x = GetPinData<float>(aNodeInstance, 3);
	const float y = GetPinData<float>(aNodeInstance, 4);
	const float z = GetPinData<float>(aNodeInstance, 5);

	gameObjects[objectIndex]->SetPosition({ x,y });*/

	return 2; // If the node doesn't have a flow, return -1 see the print node for reference
}
