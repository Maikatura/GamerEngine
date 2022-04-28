#pragma once
#include "ECS/Entity.h"
#include "ui/HierarchyPanel.h"
#include "ui/InspectorPanel.h"
#include "ui/NavigationPanel.h"
#include "ui/PropertyPanel.h"
#include "ui/ProfilerPanel.h"
#include "ui/ContentBrowserPanel.h"

namespace GamerEngine
{
	class NavigationPanel;
	class PropertyPanel;
	class ContentBrowserPanel;
	class InspectorPanel;
	class HierarchyPanel;
	class SceneView;
	class ProfilerPanel;

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
		inline static std::unique_ptr<SceneView> mySceneView;
		inline static std::unique_ptr<NavigationPanel> myNavPanel;
		inline static std::unique_ptr<PropertyPanel> mPropertyPanel;
		inline static std::unique_ptr<ContentBrowserPanel> myContentBrowserPanel;
		inline static std::unique_ptr<InspectorPanel> myInspectorPanel;
		inline static std::unique_ptr<HierarchyPanel> myHierarchyPanel;
		inline static std::unique_ptr<ProfilerPanel> myProfilerPanel;

		Entity mySelectedEntity;
	};

}
