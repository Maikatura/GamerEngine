#pragma once
#include "ECS/Entity.h"

namespace GamerEngine
{
	class SceneView;
	class Entity;

	class EngineContext
	{
	public:
		EngineContext();

		SceneView* mySceneView;
		Entity* mySelectedEntity;
	};
}
