#pragma once

#include "../../GraphNodeBase.h"
#include "../../NodeTypes.h"

class GraphNode_SetGameObjectScale : public GraphNodeBase
{
public:
	GraphNode_SetGameObjectScale();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Set Scale"; }
	virtual std::string GetNodeTypeCategory() const override { return "Utility"; }
};

