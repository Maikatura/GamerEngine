#include "pch.h"
#include "GUIHandler.h"

namespace GamerEngine
{
	GuiHandler::GuiHandler()
	{
		/*mPropertyPanel = std::make_unique<PropertyPanel>();
		myContentBrowserPanel = std::make_unique<ContentBrowserPanel>();
		myInspectorPanel = std::make_unique<InspectorPanel>();
		myNavPanel = std::make_unique<NavigationPanel>();

		mPropertyPanel->SetMeshLoadCallback(
			[this](std::string filepath) { mSceneView->LoadMesh(filepath); });*/
	}

	void GuiHandler::Update()
	{
		// UI
		//myNavPanel->OnImGuiRender();
		//mPropertyPanel->OnImGuiRender(mSceneView.get());
		//myContentBrowserPanel->OnImGuiRender();
		//myInspectorPanel->OnImGuiRender(mSceneView.get());
		// UI END
	}
}

