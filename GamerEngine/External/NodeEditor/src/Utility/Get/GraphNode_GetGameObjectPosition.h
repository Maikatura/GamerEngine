#pragma once
#include "../../GraphNodeBase.h"
#include "../../NodeTypes.h"

class GraphNode_GetGameObjectPosition : public GraphNodeBase
{
public:
	GraphNode_GetGameObjectPosition();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Get Position"; }
	virtual std::string GetNodeTypeCategory() const override { return "Utility"; }
};

