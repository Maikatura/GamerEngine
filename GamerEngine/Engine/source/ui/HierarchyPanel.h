#pragma once
#include "SceneView.h"


namespace GamerEngine
{
	class EngineContext;
	class Entity;

	class HierarchyPanel
	{
	public:

		HierarchyPanel();
		~HierarchyPanel();

		void OnImGuiRender(EngineContext* aContext);

		void RenderTreeNode(EngineContext* aContext, Entity aEntity);

	private:
		int myId;
	};
}
