#include <GraphicsEngine.pch.h>
#include <Scene/Scene.h>
#include <Model/Entity.h>
#include <Components/Components.hpp>
#include "GraphicsEngine.h"
#include "AssetHandlers/LightAssetHandler.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
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
	myDirectionalLight = nullptr;
	myEnvironmentLight = LightAssetHandler::CreateEnvironmentLight(L"Assets\\Textures\\studio_cubemap.dds");


	myComponentMap[entt::type_id<ModelComponent>().hash()] = "ModelComponent";

	return true;
}

void Scene::Clear()
{
	Renderer::Clear();
	Renderer::ResetLights();

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

	if(aEntity.HasComponent<PointLightComponent>())
	{
		Renderer::RemoveLight(aEntity.GetComponent<PointLightComponent>().myPointLight.get());
	}

	if(aEntity.HasComponent<SpotLightComponent>())
	{
		Renderer::RemoveLight(aEntity.GetComponent<SpotLightComponent>().mySpotLight.get());
	}

	if(aEntity.HasComponent<DirectionalLightComponent>())
	{
		Renderer::RemoveLight(myDirectionalLight.get());
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
		{
			const auto& view = myRegistry.view<TransformComponent, ParticleEmitter>();
			for(const auto& entity : view)
			{
				auto [transform, particleEmitter] = view.get<TransformComponent, ParticleEmitter>(entity);
				particleEmitter.OnUpdate(transform);
			}
		}

		{
			const auto& view = myRegistry.view<ModelComponent>();
			for(const auto& entity : view)
			{
				auto& component = view.get<ModelComponent>(entity);
				component.Update();
			}
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
				Renderer::SetCamera(&camera, camera.ViewProjection, camera.Projection);
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
		const auto& view = myRegistry.view<TransformComponent, DirectionalLightComponent>();
		for(const auto& entity : view)
		{
			auto [transform, dirLight] = view.get<TransformComponent, DirectionalLightComponent>(entity);

			myDirectionalLight = dirLight.myDirectionalLight;

			myDirectionalLight->SetData(&transform, &dirLight);
			
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, PointLightComponent>();
		for(const auto& entity : view)
		{
			auto [transform, pointLight] = view.get<TransformComponent, PointLightComponent>(entity);
			pointLight.myPointLight->SetData(&transform);
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, SpotLightComponent>();
		for(const auto& entity : view)
		{
			auto [transform, spotLight] = view.get<TransformComponent, SpotLightComponent>(entity);
			spotLight.mySpotLight->SetData(&transform);
		}
	}


	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		for(const auto& entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

			model.GetModel()->ClearInstanceData();
			model.GetModel()->SetHasBeenRenderer(false);
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		for(const auto& entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

			Entity entityPtr = Entity{ entity, this };
			model.GetModel()->AddRenderedInstance(&transform);

			Renderer::Render(&entityPtr, model, transform);
		}
	}

	{
		auto lightList = Renderer::GetLights();

		for(size_t i = 0; i < lightList.size(); i++)
		{
			lightList[i]->Update();
		}
		CommonUtilities::MergeSort(lightList);
		lightList;
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

void Scene::Clean()
{
	Light::Reset();
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

template<>
void Scene::OnComponentAdded<SpotLightComponent>(Entity entity, SpotLightComponent& component)
{}

template<>
void Scene::OnComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component)
{}