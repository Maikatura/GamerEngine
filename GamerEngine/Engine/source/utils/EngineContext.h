#pragma once
#include "ECS/Entity.h"

namespace GamerEngine
{
	class NavigationPanel;
	class PropertyPanel;
	class ContentBrowserPanel;
	class InspectorPanel;
	class HierarchyPanel;
	class SceneView;

	class EngineContext
	{
	public:

		EngineContext();

		void Render();

		SceneView* GetSceneView()
		{
			return mySceneView.get();
		}

		void SetEntity(Entity aEntity)
		{
			mySelectedEntity = aEntity;
		}

		Entity& GetSelectedEntity()
		{
			return mySelectedEntity;
		}

	private:
		std::unique_ptr<SceneView> mySceneView;
		std::unique_ptr<NavigationPanel> myNavPanel;
		std::unique_ptr<PropertyPanel> mPropertyPanel;
		std::unique_ptr<ContentBrowserPanel> myContentBrowserPanel;
		std::unique_ptr<InspectorPanel> myInspectorPanel;
		std::unique_ptr<HierarchyPanel> myHierarchyPanel;

		Entity mySelectedEntity;
	};

}
