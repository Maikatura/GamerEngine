#include "stdafx.h"
#include "GraphNode_MathMax.h"
#include "../GraphNodeInstance.h"
#include <Utilites/UtilityFunctions.h>

GraphNode_MathMax::GraphNode_MathMax()
{
	CreatePin("Value 1", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("Value 2", PinDirection::PinDirection_IN, DataType::Float);
	CreatePin("OUT", PinDirection::PinDirection_OUT, DataType::Float);
}

int GraphNode_MathMax::OnExec(GraphNodeInstance* aNodeInstance)
{
	const float input1 = GetPinData<float>(aNodeInstance, 0);
	const float input2 = GetPinData<float>(aNodeInstance, 1);
	const float result = CommonUtilities::Max(input1, input2);
	SetPinData(aNodeInstance, 2, result);

	return -1;
}
