#pragma once
#include "stdafx.h"
#include "../GraphNodeInstance.h"

class GraphNode_MathSin : public GraphNodeBase
{
public:
	GraphNode_MathSin();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Sin"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};