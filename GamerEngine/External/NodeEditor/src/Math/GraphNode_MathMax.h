#pragma once
#include "../../GraphNodeBase.h"
#include "../../NodeTypes.h"

class GraphNode_MathMax : public GraphNodeBase
{
public:
	GraphNode_MathMax();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Max"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};

