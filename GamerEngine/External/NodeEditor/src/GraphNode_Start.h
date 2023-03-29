#pragma once
#include "GraphNodeBase.h"
#include "NodeTypes.h"

/**
 * This node is the entry point of a Node Graph. There should only be one per graph
 * though if you feel so inclined you can derive from it and modify the graph system
 * to allow for different entry points in the same graph. I.e. like UE does with
 * various events in an actor blueprint graph.
 */
class GraphNode_Start : public GraphNodeBase
{
public:
	GraphNode_Start();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	bool IsStartNode() const override { return true; }

	virtual std::string GetNodeName() const override { return "Start"; }	
};

