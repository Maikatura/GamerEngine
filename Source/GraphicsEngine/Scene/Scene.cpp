#include <GraphicsEngine.pch.h>
#include <Scene/Scene.h>
#include <Model/Entity.h>
#include <Components/Components.hpp>
#include <Components/ScriptableEntity.h>

#include "GraphicsEngine.h"
#include "SceneSerializer.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/Animator.h"
#include "Components/CameraController.h"
#include "Components/Rotater.h"
#include "Render/Renderer.h"

Scene::~Scene()
{
	Clear();
}

bool Scene::Initialize()
{
	return true;
}

void Scene::Clear()
{
	myRegistry.each([&](auto entityID)
		{
			myRegistry.destroy(entityID);
		});
}

void Scene::Resize(Vector2ui aNewWindowSize)
{
	auto view = myRegistry.view<CameraComponent>();
	for(auto entity : view)
	{
		auto& camera = view.get<CameraComponent>(entity);
		camera.Resize(aNewWindowSize);
	}
}

entt::registry& Scene::GetRegistry()
{
	return myRegistry;
}

Entity Scene::CreateEntity(const std::string& aName)
{
	return CreateEntityWithUUID(UUID2(), aName);
}


Entity Scene::CreateEntityWithUUID(UUID2 aUUID, const std::string& aName)
{
	Entity entity = { myRegistry.create(), this };
	auto& idComp = entity.AddComponent<IDComponent>();
	idComp.ID = aUUID;
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = aName.empty() ? "Entity" : aName;
	return entity;
}

void Scene::DeleteEntity(Entity aEntity)
{
	myRegistry.destroy(aEntity);
}

void Scene::OnUpdate()
{
	{
		myRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if(!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->myEntity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate();
			});
	}

	{
		/*auto view = myRegistry.view<TransformComponent, Animator>();
		for(auto entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, Animator>(entity);
		}*/
	}
}

void Scene::OnRender()
{
	{
		auto view = myRegistry.view<TransformComponent, CameraComponent>();
		for(auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if(camera.Primary)
			{
				Renderer::SetCamera(camera.ViewProjection, camera.Projection);
				break;
			}
		}
	}

	{
		auto view = myRegistry.view<TransformComponent, ModelComponent>();
		for(auto entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

			Renderer::Render(model, transform);
		}
	}
}

const std::string& Scene::GetPath()
{
	return myPath;
}

void Scene::SetPath(const std::string& aPath)
{
	myPath = aPath;
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component)
{
	//static_assert(sizeof(T) == 0);
}

template<>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
{}

template<>
void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
{}

template<>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
{}

template<>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{}

template<>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
{}

template<>
void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
{}