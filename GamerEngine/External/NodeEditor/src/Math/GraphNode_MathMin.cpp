#include "stdafx.h"
#include "GraphNode_MathMin.h"
#include "../GraphNodeInstance.h"
#include <Utilites/UtilityFunctions.h>

GraphNode_MathMin::GraphNode_MathMin()
{
	CreatePin("Value 1", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Value 2", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("OUT", PinDirection::PinDirection_OUT, DataType::Float);
}

int GraphNode_MathMin::OnExec(GraphNodeInstance* aNodeInstance)
{
	const float input1 = GetPinData<float>(aNodeInstance, 0);
	const float input2 = GetPinData<float>(aNodeInstance, 1);
	const float result = CommonUtilities::Min(input1, input2);
	SetPinData(aNodeInstance, 2, result);

	return -1;
}
