#include "stdafx.h"
#include "GraphNode_MathLength.h"
#include "../GraphNodeInstance.h"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

GraphNode_MathLength::GraphNode_MathLength()
{
	CreatePin("X 1", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Y 1", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("X 2", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Y 2", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("OUT", PinDirection::PinDirection_OUT, DataType::Float);
}

int GraphNode_MathLength::OnExec(GraphNodeInstance* aNodeInstance)
{
	// Get data on pin index 0, this index is relative to what you push in the constructor
	const float x1 = GetPinData<float>(aNodeInstance, 0);
	const float y1 = GetPinData<float>(aNodeInstance, 1);
	const float x2 = GetPinData<float>(aNodeInstance, 2);
	const float y2 = GetPinData<float>(aNodeInstance, 3);
	
	CommonUtilities::Vector2<float> position1{ x1 ,y1 };
	CommonUtilities::Vector2<float> position2{ x2 ,y2 };
	
	// Perform the Add operation we're supposed to do.
	const float result = CommonUtilities::Vector2<float>::Distance(position1, position2);

	// Set the result we calculated to the data property on the Result pin.
	SetPinData(aNodeInstance, 4, result);

	return -1; // If the node doesn't have a flow, return -1 see the print node for reference
}
