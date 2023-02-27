#pragma once
#include "../../GraphNodeBase.h"
#include "../../NodeTypes.h"

class GraphNode_GetMousePosition : public GraphNodeBase
{
public:
	GraphNode_GetMousePosition();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Get Mouse Position"; }
	virtual std::string GetNodeTypeCategory() const override { return "Utility"; }
};

