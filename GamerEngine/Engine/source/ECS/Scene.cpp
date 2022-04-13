#include "pch.h"
#include "Scene.h"
#include "../Components/TransformComponent.h"

namespace GamerEngine
{
	Entity* Scene::CreateEntity()
	{
		entt::entity entity = myRegistry.create();
		myRegistry.emplace<TransformComponent>(entity);
		auto myEntity = new Entity(entity);

		return myEntity;
	}
}
