#include "pch.h"
#include "utils/EngineContext.h"
#include "ui/SceneView.h"
#include "ECS/Entity.h"

namespace GamerEngine
{
	EngineContext::EngineContext() : mySelectedEntity(entt::null), mySceneView(nullptr)
	{
	}
}
