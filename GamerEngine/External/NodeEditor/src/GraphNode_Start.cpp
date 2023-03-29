#include "stdafx.h"
#include "GraphNode_Start.h"
#include <iostream>
#include "GraphNodeBase.h"
#include "GraphNodeInstance.h"


GraphNode_Start::GraphNode_Start()
{
	CreatePin("", PinDirection::PinDirection_OUT);
	CreatePin("Updating", PinDirection::PinDirection_IN, DataType::Bool);	
}

int GraphNode_Start::OnExec(class GraphNodeInstance* aNodeInstance)
{
	const bool shouldRepeat = GetPinData<bool>(aNodeInstance, 1);
	aNodeInstance->SetShouldTriggerAgain(shouldRepeat);
	return 0;
}
