#include "pch.h"
#include "Scene.h"

#include "Components/MeshComponent.h"

namespace GamerEngine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Render(GamerEngine::Shader* shader)
	{
		
		/*auto view = myRegistry.view<MeshComponent>();

		for (auto entity : view)
		{
			auto& mesh = myRegistry.get<MeshComponent>(entity);
			mesh.myModel.Update(shader);
		}*/
	}
}
