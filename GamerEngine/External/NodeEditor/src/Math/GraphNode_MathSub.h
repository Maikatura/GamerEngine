#pragma once
#include "../GraphNodeBase.h"
#include "../NodeTypes.h"

class GraphNode_MathSub : public GraphNodeBase
{
public:
	GraphNode_MathSub();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Subtract"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};

