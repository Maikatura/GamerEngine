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

#include "SceneManager.h"
#include "..\..\..\Game\src\Components\TransfromComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/Network/NetworkComponent.h"
#include "Math/AABB3D.hpp"
#include "TurNet/Shared/DataHandling/TurMessage.h"

//#include "flecs.h"


Scene::Scene() : mySceneIsReady(false), mySceneStatus(SceneStatus::None)
{

}

Scene::~Scene()
{
	//myWorld.quit();


	Clear();
}

bool Scene::Initialize()
{
	myDirectionalLight = nullptr;
	myEnvironmentLight = LightAssetHandler::CreateEnvironmentLight(L"resources\\textures\\studio_cubemap.dds");

	myComponentMap[entt::type_id<ModelComponent>().hash()] = "ModelComponent";

	/*myWorld.set<flecs::Rest>({});
	myWorld.import<flecs::monitor>();

	myStatisticThread = std::thread([&]()
	{
		myWorld.app().enable_rest().run();
	});*/




	return true;
}

void Scene::Clear()
{
	//mySceneIsReady = false;

	Renderer::Clear();
	myLightToRender.clear();


	/*myRegistry.each([&](auto entityID)
		{
			myRegistry.destroy(entityID);

			Entity aEntity{ entityID , this };

			if(aEntity.HasComponent<NativeScriptComponent>())
			{
				aEntity.GetComponent<NativeScriptComponent>().DestroyScript;
			}
		});*/
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


Entity Scene::CreateEntityWithUUID(const UUID2& aUUID, const std::string& aName)
{
	Entity entity = { myRegistry.create(), this };
	auto& idComp = entity.AddComponent<IDComponent>(aUUID);
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = aName.empty() ? "Default Entity Name" : aName;

	if(aName == "EditorCamera")
	{
		if(myEditorCamHandler)
		{
			myEditorCamHandler(entity);
		}
	}

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
		for(int i = 0; i < myLightToRender.size(); i++)
		{
			if(myLightToRender[i] == aEntity.GetComponent<PointLightComponent>().myPointLight.get())
			{
				myLightToRender.erase(myLightToRender.begin() + i);
			}
		}
	}

	if(aEntity.HasComponent<SpotLightComponent>())
	{
		for(int i = 0; i < myLightToRender.size(); i++)
		{
			if(myLightToRender[i] == aEntity.GetComponent<SpotLightComponent>().mySpotLight.get())
			{
				myLightToRender.erase(myLightToRender.begin() + i);
			}
		}
	}

	if(aEntity.HasComponent<DirectionalLightComponent>())
	{
		for(int i = 0; i < myLightToRender.size(); i++)
		{
			if(myLightToRender[i] == myDirectionalLight.get())
			{
				myLightToRender.erase(myLightToRender.begin() + i);
			}
		}
	}

	myRegistry.destroy(aEntity);
}

void Scene::OnUpdate(bool aShouldRunLoop, bool aLoadingScene)
{
	if(!mySceneIsReady) return;
	if(mySceneStatus != SceneStatus::Complete) return;

	
	{
		myRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if(!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->myEntity = Entity{ entity, this };
				nsc.Instance->OnCreate();
			}

			if(nsc.Instance)
			{
				if((aShouldRunLoop || nsc.Instance->RunInEditor()))
				{
					nsc.Instance->OnUpdate();
				}
			}

		});
	}

	{
		const auto& view = myRegistry.view<ModelComponent>();

		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto& component = view.get<ModelComponent>(entity);
				component.OnEditorUpdate();
			}
		}
	}

	if(aShouldRunLoop)
	{
		{
			const auto& view = myRegistry.view<Network::NetworkComponent>();

			if(view != nullptr)
			{

				for(const auto& entity : view)
				{
					auto& networkComponent = view.get<Network::NetworkComponent>(entity);
					auto aEntity = Entity{ entity, this };

					networkComponent.SetEntity(&aEntity);
					networkComponent.OnUpdate();
				}
			}
		}

		{
			const auto& view = myRegistry.view<TransformComponent, ParticleEmitter>();

			if(view != nullptr)
			{

				for(const auto& entity : view)
				{
					auto [transform, particleEmitter] = view.get<TransformComponent, ParticleEmitter>(entity);
					particleEmitter.OnUpdate(transform);
				}
			}
		}

		{
			const auto& view = myRegistry.view<ModelComponent>();

			if(view != nullptr)
			{
				for(const auto& entity : view)
				{
					auto& component = view.get<ModelComponent>(entity);
					component.OnUpdate();
				}
			}
		}
	}
}

void Scene::OnRender()
{
	if(mySceneStatus != SceneStatus::Complete) return;
	if (!mySceneIsReady) return;

	{
		Clear();
	}

	{
		const auto& view = myRegistry.view<TransformComponent, CameraComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.Primary)
				{
					camera.BuildTransform(&transform);
					Renderer::SetCamera(&camera, camera.ViewProjection, camera.Projection);
					break;
				}
			}
		}
	}

	{
	
		const auto& view = myRegistry.view<TransformComponent, ParticleEmitter>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, particleEmitter] = view.get<TransformComponent, ParticleEmitter>(entity);
				Renderer::RenderSprite(&particleEmitter, transform);
			}
		}
	}
	{
		const auto& view = myRegistry.view<TransformComponent, DirectionalLightComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, dirLight] = view.get<TransformComponent, DirectionalLightComponent>(entity);

				myDirectionalLight = dirLight.myDirectionalLight;
				myDirectionalLight->SetData(&transform, &dirLight);
				RenderLight(myDirectionalLight.get());
			}
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, PointLightComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, pointLight] = view.get<TransformComponent, PointLightComponent>(entity);
				if(pointLight.myPointLight)
				{
					pointLight.myPointLight->SetData(&transform);
					RenderLight(pointLight.myPointLight.get());
					
				}
			}
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, SpotLightComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, spotLight] = view.get<TransformComponent, SpotLightComponent>(entity);
				if(spotLight.mySpotLight)
				{
					spotLight.mySpotLight->SetData(&transform);
					RenderLight(spotLight.mySpotLight.get());
				}
			}
		}
	}


	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

				if(model.GetModel())
				{
					model.GetModel()->ClearInstanceData();
					model.GetModel()->SetHasBeenRenderer(false);
				}

			}
		}
	}

	{
		const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
		if(view != nullptr)
		{
			for(const auto& entity : view)
			{
				auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

				if (!model.IsLoaded())
				{
					model.GetDelay() -= Time::GetDeltaTime();

					if (model.GetDelay() <= 0.0f)
					{
						model.SetLoaded(true);
					}

					continue;
				}

				if(model.GetModel())
				{

					/*const auto& realModel =  model.GetModel()->GetModel();
					for(int i = 0; i < model.GetModel()->GetNumMeshes(); i++)
					{
						auto transformedBounds = CommonUtilities::AABB({
								realModel->BoxBounds.Center[0],
								realModel->BoxBounds.Center[1],
								realModel->BoxBounds.Center[2]
							},

							{
								realModel->BoxBounds.BoxExtents[0],
								realModel->BoxBounds.BoxExtents[1],
								realModel->BoxBounds.BoxExtents[2]
							}).Transform(transform.GetMatrix());
						if(transformedBounds.IsOnFrustum(Renderer::GetCamera()->myFrustum))
						{*/
							Entity entityPtr = Entity{ entity, this };
							model.GetModel()->AddRenderedInstance(&transform);
							Renderer::Render(&entityPtr, model, transform);
						//}
					//}
				}
			}
		}
	}

	{
		//myLightToRender.push_back(myEnvironmentLight.get());
		for(size_t i = 0; i < myLightToRender.size(); i++)
		{
			myLightToRender[i]->Update();
		}
		CommonUtilities::MergeSort(myLightToRender);
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

void Scene::SceneReady(bool isSceneReady)
{
	mySceneIsReady = isSceneReady;
}

void Scene::Clean()
{
	Light::Reset();
}

std::vector<Light*> Scene::GetSomeLights()
{
	std::vector<Light*> myList;

	for(size_t i = 0; i < GetLights().size(); i++)
	{
		if(GetLights()[i]->GetLightBufferData().LightType != 1)
		{
			myList.push_back(GetLights()[i]);
		}
	}
	return myList;
}

std::vector<Light*>& Scene::GetLights()
{
	CommonUtilities::MergeSortPtr(myLightToRender);
	return myLightToRender;
}

void Scene::RemoveLight(Light* aLight)
{
	for(int i = 0; i < myLightToRender.size(); i++)
	{
		if(myLightToRender[i] == aLight)
		{
			myLightToRender.erase(myLightToRender.begin() + i);
			i--;
		}
	}
}

void Scene::ResetLights()
{
	myLightToRender.clear();
}

void Scene::RenderLight(Light* aLight)
{
	myLightToRender.push_back(aLight);
}

Entity Scene::GetNetworkEntity(TurNet::TurMessage* aMessage)
{

	int networkID;
	*aMessage >> networkID;


	Entity entityReturn;
	const auto& view = myRegistry.view<Network::NetworkComponent>();


	for(const auto& entity : view)
	{
		auto& component = view.get<Network::NetworkComponent>(entity);

		if(component.GetID() == networkID)
		{
			entityReturn = Entity(entity, this);

		}
	}

	return entityReturn;
}

void Scene::SetSceneStatus(SceneStatus aSceneStatus)
{
	mySceneStatus = aSceneStatus;
}

SceneStatus Scene::GetSceneStatus()
{
	return mySceneStatus;
}

bool Scene::IsReady()
{
	return mySceneIsReady;
}

void Scene::SetCameraHandle(std::function<void(Entity)> aCameraHandle)
{
	myEditorCamHandler = aCameraHandle;
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