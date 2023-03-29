#pragma once
#include <imgui_node_editor.h>

#include "GraphVariable.h"

namespace ed = ax::NodeEditor;
class GraphManager
{
public:
	GraphManager();
	~GraphManager();
	void Load();

	void ReTriggerUpdateringTrees();
	
	void PreFrame(float aTimeDelta);
	void ConstructEditorTreeAndConnectLinks();
	void PostFram();

	void ReTriggerTree(std::string aSpecificNodeName = std::string("Start"));
	void ExecuteTreeWithPayload(std::string aStartNodeName, Payload aPayload);
	void ExecuteTreeWithPayload(std::tuple<std::string, Payload> aTuple);
	void SaveTreeToFile();
	void LoadTreeFromFile();
	static void ShowFlow(int aLinkID);

private:
	void WillBeCyclic(class GraphNodeInstance* aFirst, bool& aIsCyclic, class GraphNodeInstance* aBase);
	
	class GraphNodeInstance* GetNodeFromPinID(unsigned int aID);
	GraphNodeInstance* GetNodeFromNodeID(unsigned int aID);
	void DrawTypeSpecificPin(struct GraphNodePin& aPin, class GraphNodeInstance* aNodeInstance);
	std::vector<class GraphNodeInstance*> myNodeInstancesInGraph;

	struct EditorLinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};

	ImVector<EditorLinkInfo> myLinks;
	int myNextLinkIdCounter = 100;
	bool myLikeToSave = false;
	bool myLikeToShowFlow = false;
	char* myMenuSeachField = nullptr;
	bool mySetSearchFokus = true;
	bool myShouldCreateVariable = false;
	char* myNewVarName = DBG_NEW char[255]();
	std::string myNewVarTypeLabel = "Select Type...";
	DataType myNewVarType = DataType::Unknown;
	int mySelectedVariableIndex = -1;
	std::string myVariableErrorMessage;

	std::vector<GraphVariable> myGraphVariables;
};
