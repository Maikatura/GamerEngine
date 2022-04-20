#pragma once
#include "SceneView.h"


namespace GamerEngine
{
	class EngineContext;

	class HierarchyPanel
	{
	public:

		HierarchyPanel();
		~HierarchyPanel();

		void OnImGuiRender(SceneView* aContext);

		void RenderTreeNode(SceneView* aContext, entt::entity& aEntity);

	private:
		int myId;
	};
}
