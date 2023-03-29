#include "stdafx.h"
#include "GraphNode_MathAbs.h"
#include "../GraphNodeInstance.h"

GraphNode_MathAbs::GraphNode_MathAbs()
{
	CreatePin("Value 1", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("OUT", PinDirection::PinDirection_OUT, DataType::Float);
}

int GraphNode_MathAbs::OnExec(GraphNodeInstance* aNodeInstance)
{
	// Get data on pin index 0, this index is relative to what you push in the constructor
	const float input1 = GetPinData<float>(aNodeInstance, 0);

	// Perform the Add operation we're supposed to do.
	const float result = std::abs(input1);

	// Set the result we calculated to the data property on the Result pin.
	SetPinData(aNodeInstance, 2, result);

	return -1; // If the node doesn't have a flow, return -1 see the print node for reference
}
