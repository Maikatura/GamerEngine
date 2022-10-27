#include <GraphicsEngine.pch.h>
#include <Scene/Scene.h>
#include <Model/Entity.h>
#include <Components/Components.hpp>
#include "GraphicsEngine.h"
#include "AssetHandlers/LightAssetHandler.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include <Model/ModelInstance.h>
#include "Particles/ParticleEmitter.h"
#include "Render/Renderer.h"
#include <Components/ScriptableEntity.h>

#include "Components/ChildComponent.h"

Scene::~Scene()
{
	Clear();
}

bool Scene::Initialize()
{
	myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 2.0f, { 0,90,0 });
	//myEnvironmentLight = LightAssetHandler::CreateEnvironmentLight(L"Assets\\Textures\\studio_cubemap.dds");

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
	entity.AddComponent<ChildComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = aName.empty() ? "Default Entity Name" : aName;
	return entity;
}

void Scene::DeleteEntity(Entity aEntity)
{
	if(aEntity.HasComponent<NativeScriptComponent>())
	{
		aEntity.GetComponent<NativeScriptComponent>().DestroyScript;
	}

	myRegistry.destroy(aEntity);
}

void Scene::OnUpdate(bool aShouldRunLoop)
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

	if(aShouldRunLoop)
	{

		const auto& view = myRegistry.view<TransformComponent, ParticleEmitter>();
		for(const auto& entity : view)
		{
			auto [transform, particleEmitter] = view.get<TransformComponent, ParticleEmitter>(entity);
			//model.myModel->GetMatrix().GetMatrix().BuildTransform(transform.Translation, transform.Rotation, transform.Scale);
			particleEmitter.OnUpdate(transform);
		}
	}

	if(aShouldRunLoop)
	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		for(const auto& entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);
			//model.myModel->GetMatrix().GetMatrix().BuildTransform(transform.Translation, transform.Rotation, transform.Scale);
			model.myModel->Update();

		}
	}
}

void Scene::OnRender()
{
	{
		Renderer::SetClearColor(mySceneBackgroundColor);
	}

	{
		const auto& view = myRegistry.view<TransformComponent, CameraComponent>();
		for(const auto& entity : view)
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
		const auto& view = myRegistry.view<TransformComponent, ParticleEmitter>();
		for(const auto& entity : view)
		{
			auto [transform, particleEmitter] = view.get<TransformComponent, ParticleEmitter>(entity);
			Renderer::RenderSprite(&particleEmitter, transform);
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		for(const auto& entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

			Entity entityPtr = Entity{ entity, this };
			Renderer::Render(&entityPtr, model, transform);
		}
	}
}

const std::string& Scene::GetPath()
{
	return myPath;
}

std::shared_ptr<DirectionalLight> Scene::GetDirLight()
{
	return myDirectionalLight;
}

std::shared_ptr<EnvironmentLight> Scene::GetEnvLight()
{
	return myEnvironmentLight;
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

template<>
void Scene::OnComponentAdded<ParticleEmitter>(Entity entity, ParticleEmitter& component)
{}

template<>
void Scene::OnComponentAdded<DirectionalLightComponent>(Entity entity, DirectionalLightComponent& component)
{}