#pragma once
#include "../GraphNodeBase.h"
#include "../NodeTypes.h"

class GraphNode_MathAdd : public GraphNodeBase
{
public:
	GraphNode_MathAdd();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Add"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};

