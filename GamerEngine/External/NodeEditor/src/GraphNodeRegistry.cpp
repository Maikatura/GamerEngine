#include "stdafx.h"
#include "GraphNodeRegistry.h"

#include "GraphNode_Print.h"
#include "GraphNode_Variable.h"
#include "GraphNode_Start.h"

#include "Math/GraphNode_MathAdd.h"
#include "Math/GraphNode_MathSub.h"
#include "Math/GraphNode_MathMul.h"
#include "Math/GraphNode_MathCos.h"
#include "Math/GraphNode_MathSin.h"
#include "Math/GraphNode_MathAbs.h"
#include "Math/GraphNode_MathMin.h"
#include "Math/GraphNode_MathMax.h"
#include "math/GraphNode_MathAtan2.h"
#include "Math/GraphNode_MathLength.h"

#include "Utility/Get/GraphNode_GetGameObjectPosition.h"
#include "Utility/Get/GraphNode_GetMousePosition.h"

#include "Utility/Set/GraphNode_SetGameObjectPosition.h"
#include "Utility/Set/GraphNode_SetGameObjectScale.h"


void GraphNodeRegistry::PopulateTypes()
{
	RegisterType<GraphNode_Print>();
	RegisterType<GraphNode_Start>();

	RegisterType<GraphNode_MathAdd>();
	RegisterType<GraphNode_MathSub>();
	RegisterType<GraphNode_MathMul>();
	RegisterType<GraphNode_MathCos>();
	RegisterType<GraphNode_MathSin>();
	RegisterType<GraphNode_MathAbs>();
	RegisterType<GraphNode_MathAtan2>();
	RegisterType<GraphNode_MathLength>();
	RegisterType<GraphNode_MathMin>();
	RegisterType<GraphNode_MathMax>();

	RegisterType<GraphNode_GetGameObjectPosition>();
	RegisterType<GraphNode_SetGameObjectPosition>();

	RegisterType<GraphNode_SetGameObjectScale>();

	RegisterType<GraphNode_GetMousePosition>();

	RegisterType<GraphNode_Set>();
	RegisterType<GraphNode_Get>();




}

GraphNodeBase* GraphNodeRegistry::myTypes[128];
unsigned short GraphNodeRegistry::myTypeCounter = 0;