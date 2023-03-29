#pragma once
#include "../../GraphNodeBase.h"
#include "../../NodeTypes.h"

class GraphNode_SetGameObjectPosition : public GraphNodeBase
{
public:
	GraphNode_SetGameObjectPosition();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Set Position"; }
	virtual std::string GetNodeTypeCategory() const override { return "Utility"; }
};

