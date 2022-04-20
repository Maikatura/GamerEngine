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

		void OnImGuiRender(EngineContext* aContext);

		void RenderTreeNode(EngineContext* aContext, entt::entity& aEntity);

	private:
		int myId;
	};
}
