#pragma once
#include "stdafx.h"
#include "GraphNode_MathMul.h"
#include "../GraphNodeInstance.h"

class GraphNode_MathAbs : public GraphNodeBase
{
public:
	GraphNode_MathAbs();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Abs"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};