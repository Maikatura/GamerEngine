#include "pch.h"
#include "EngineContext.h"
#include "ui/SceneView.h"

namespace GamerEngine
{
	EngineContext::EngineContext()
	{
		mySceneView = std::make_unique<SceneView>();
		mPropertyPanel = std::make_unique<PropertyPanel>();
		myContentBrowserPanel = std::make_unique<ContentBrowserPanel>();
		myInspectorPanel = std::make_unique<InspectorPanel>();
		myNavPanel = std::make_unique<NavigationPanel>();
		myHierarchyPanel = std::make_unique<HierarchyPanel>();
		myProfilerPanel = std::make_unique<ProfilerPanel>();

		mPropertyPanel->SetMeshLoadCallback(
			[this](std::string filepath) { mySceneView->LoadMesh(filepath); });
	}

	void EngineContext::Render()
	{
		mySceneView->Render();

		myNavPanel->OnImGuiRender();
		mPropertyPanel->OnImGuiRender(mySceneView.get());
		myContentBrowserPanel->OnImGuiRender();
		myInspectorPanel->OnImGuiRender(this);
		myHierarchyPanel->OnImGuiRender(this);
		myProfilerPanel->OnImGuiRender();

	}
}
