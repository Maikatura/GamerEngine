#pragma once
#include "stdafx.h"
#include "GraphNode_MathMul.h"
#include "../GraphNodeInstance.h"

class GraphNode_MathCos : public GraphNodeBase
{
public:
	GraphNode_MathCos();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Cos"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};