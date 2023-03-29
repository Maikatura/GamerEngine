#pragma once
#include "../GraphNodeBase.h"
#include "../NodeTypes.h"

class GraphNode_MathMul : public GraphNodeBase
{
public:
	GraphNode_MathMul();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Multiply"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};

