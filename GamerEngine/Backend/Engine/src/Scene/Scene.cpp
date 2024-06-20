#include <GraphicsEngine.pch.h>
#include <Scene/Scene.h>
#include <Core/Model/Entity.h>
#include "AssetHandlers/LightAssetHandler.h"
#include <Core/Model/Model.h>
#include "Particles/ParticleEmitter.h"
#include "Core/Rendering/Renderer.h"
#include "Components/AllComponents.h"
#include "Components/NativeScriptComponent.h"
#include "Core/Rendering/LineRenderer.h"
#include "Scripting/ScriptEngine.h"

//#include "flecs.h"


GamerEngine::Scene::Scene() : mySceneStatus(SceneStatus::None)
{
   
}

GamerEngine::Scene::~Scene()
{
    Clear();
}

bool GamerEngine::Scene::Initialize()
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

void GamerEngine::Scene::Clear()
{
    //mySceneIsReady = false;

    //Renderer::Clear();
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

void GamerEngine::Scene::Resize(Vector2ui aNewWindowSize)
{
    const auto view = myRegistry.view<CameraComponent>();
    for (const auto entity : view)
    {
        auto& camera = view.get<CameraComponent>(entity);
        camera.Resize(aNewWindowSize);
    }

    myEditorCamera.myCamera->Resize(aNewWindowSize);
}

entt::registry& GamerEngine::Scene::GetRegistry()
{
    return myRegistry;
}

GamerEngine::Entity GamerEngine::Scene::CreateEntity(const std::string& aName)
{
    return CreateEntityWithUUID(UUID(), aName);
}


GamerEngine::Entity GamerEngine::Scene::CreateEntityWithUUID(UUID aUUID, const std::string& aName)
{
    Entity entity = {myRegistry.create(), this };
    entity.AddComponent<IDComponent>(aUUID);
    entity.AddComponent<GamerEngine::TransformComponent>();
    //entity.AddComponent<GamerEngine::ScriptComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = aName.empty() ? "Default Entity Name" : aName;

    myEntityMap[aUUID] = entity;

    if (aName == "EditorCamera")
    {
        if (myEditorCamHandler)
        {
            myEditorCamHandler(entity);
        }
    }

    return entity;
}

void GamerEngine::Scene::DeleteEntity(Entity aEntity)
{
    if (aEntity.HasComponent<NativeScriptComponent>())
    {
        aEntity.GetComponent<NativeScriptComponent>().DestroyInstanceFunction();
    }

    if (aEntity.HasComponent<PointLightComponent>())
    {
        auto& pointLight = aEntity.GetComponent<PointLightComponent>().myPointLight;
        auto it = std::find(myLightToRender.begin(), myLightToRender.end(), pointLight.get());
        if (it != myLightToRender.end())
        {
            std::iter_swap(it, myLightToRender.end() - 1);
            myLightToRender.pop_back();
        }
    }

    if (aEntity.HasComponent<SpotLightComponent>())
    {
        auto& pointLight = aEntity.GetComponent<SpotLightComponent>().mySpotLight;
        auto it = std::find(myLightToRender.begin(), myLightToRender.end(), pointLight.get());
        if (it != myLightToRender.end())
        {
            std::iter_swap(it, myLightToRender.end() - 1);
            myLightToRender.pop_back();
        }
    }

    if (aEntity.HasComponent<DirectionalLightComponent>())
    {
        auto& pointLight = aEntity.GetComponent<DirectionalLightComponent>().myDirectionalLight;
        auto it = std::find(myLightToRender.begin(), myLightToRender.end(), pointLight.get());
        if (it != myLightToRender.end())
        {
            std::iter_swap(it, myLightToRender.end() - 1);
            myLightToRender.pop_back();
        }
    }

    myEntityMap.erase(aEntity.GetUUID());
    myRegistry.destroy(aEntity);
}


template<typename... Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<GamerEngine::UUID, entt::entity>& enttMap)
{
    ([&]()
    {
        auto view = src.view<Component>();
        for (auto srcEntity : view)
        {
            entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

            auto& srcComponent = src.get<Component>(srcEntity);
            dst.emplace_or_replace<Component>(dstEntity, srcComponent);
        }
    }(), ...);
}

template<typename... Component>
static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<GamerEngine::UUID, entt::entity>& enttMap)
{
    CopyComponent<Component...>(dst, src, enttMap);
}

template<typename... Component>
static void CopyComponentIfExists(GamerEngine::Entity dst, GamerEngine::Entity src)
{
    ([&]()
    {
        if (src.HasComponent<Component>())
            dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
    }(), ...);
}

template<typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, GamerEngine::Entity dst, GamerEngine::Entity src)
{
    CopyComponentIfExists<Component...>(dst, src);
}

Ref<GamerEngine::Scene> GamerEngine::Scene::Copy(Ref<Scene> other)
{
    Ref<Scene> newScene = MakeRef<Scene>();


    auto& srcSceneRegistry = other->myRegistry;
    auto& dstSceneRegistry = newScene->myRegistry;
    std::unordered_map<GamerEngine::UUID, entt::entity> enttMap;

    // Create entities in new scene
    auto idView = srcSceneRegistry.view<IDComponent>();
    for (auto e : idView)
    {
		GamerEngine::UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
        const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
        Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
        enttMap[uuid] = (entt::entity)newEntity;
    }

    // Copy components (except IDComponent and TagComponent)
    CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

    return newScene;
}

void GamerEngine::Scene::OnRuntimeStart()
{
    ScriptEngine::OnRuntimeStart(this);
    // Instantiate all script entities

    auto view = myRegistry.view<ScriptComponent>();
    for (auto e : view)
    {
        Entity entity = { e, this };
        ScriptEngine::OnCreateEntity(entity);
    }
}

void GamerEngine::Scene::OnRuntimeStop()
{
    ScriptEngine::OnRuntimeStop();
}

void GamerEngine::Scene::OnUpdate(bool aShouldRunLoop, bool aLoadingScene)
{
    if (!mySceneIsReady) return;
    if (mySceneStatus != SceneStatus::Complete) return;


    {
        const auto& view = myRegistry.view<ModelComponent>();

        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto& component = view.get<ModelComponent>(entity);
                component.OnEditorUpdate();
            }
        }
    }

    if (aShouldRunLoop)
    {
        {

            auto view = myRegistry.view<ScriptComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                ScriptEngine::OnUpdateEntity(entity, Time::GetDeltaTime());
            }

            myRegistry.view<NativeScriptComponent>().each([this](auto entity, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    nsc.InstantiateFunction();
                    nsc.Instance->myEntity = {entity, this };
                    nsc.OnCreateFunction(nsc.Instance);
                }

                nsc.OnUpdateFunction(nsc.Instance, Time::GetDeltaTime());
            });
        }



        {
            const auto& view = myRegistry.view<Network::NetworkComponent>();

            if (view != nullptr)
            {
                for (const auto& entity : view)
                {
                    auto& networkComponent = view.get<Network::NetworkComponent>(entity);
                    auto aEntity = Entity{entity, this };

                    // networkComponent.SetEntity(&aEntity);
                    // networkComponent.OnUpdate();
                }
            }
        }

        {
            const auto& view = myRegistry.view<GamerEngine::TransformComponent, ParticleEmitter>();

            if (view != nullptr)
            {
                for (const auto& entity : view)
                {
                    auto [transform, particleEmitter] = view.get<GamerEngine::TransformComponent, ParticleEmitter>(entity);
                    particleEmitter.OnUpdate(transform);
                }
            }
        }

        {
            const auto& view = myRegistry.view<ModelComponent>();

            if (view != nullptr)
            {
                for (const auto& entity : view)
                {
                    auto& component = view.get<ModelComponent>(entity);
                    component.OnUpdate();
                }
            }
        }

        {
            const auto& view = myRegistry.view<GamerEngine::TransformComponent, CameraComponent>();
            if (view != nullptr)
            {
                for (const auto& entity : view)
                {
                    auto [transform, camera] = view.get<GamerEngine::TransformComponent, CameraComponent>(entity);

                    if (camera.Primary)
                    {
                        camera.BuildTransform(&transform);
                        Renderer::SetCamera(&camera, transform, camera.ViewProjection, camera.Projection);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        myEditorCamera.Update();
    }

    

    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent, ModelComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, model] = view.get<GamerEngine::TransformComponent, ModelComponent>(entity);

                if (!model.IsLoaded())
                {
                    model.GetDelay() -= Time::GetDeltaTime();

                    if (model.GetDelay() <= 0.0f)
                    {
                        model.SetLoaded(true);
                    }

                    continue;
                }

                if (model.GetModel())
                {
                    model.OnUpdate();

                    Entity entityPtr = Entity{entity, this };
                    Renderer::Render(&entityPtr, model, transform);
                }
            }
        }
    }
}

void GamerEngine::Scene::OnRender()
{
    if (mySceneStatus != SceneStatus::Complete) return;
    if (!mySceneIsReady) return;

    {
        Clear();
    }

    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto& transform = view.get<GamerEngine::TransformComponent>(entity);
                transform.BuildTransform();
            }
        }
    }


    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent, ParticleEmitter>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, particleEmitter] = view.get<GamerEngine::TransformComponent, ParticleEmitter>(entity);
                Renderer::RenderSprite(&particleEmitter, transform);
            }
        }
    }
    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent, DirectionalLightComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, dirLight] = view.get<GamerEngine::TransformComponent, DirectionalLightComponent>(entity);
                if (dirLight.GetLight())
                {
                    if (!dirLight.Active)
                    {
                        continue;
                    }

                    dirLight.GetLight()->SetData(&transform);
                    dirLight.OnUpdate();
                    RenderLight(dirLight.GetLight());
                }
            }
        }
        else
        {
            SetDirectionalLight(nullptr);
        }
    }

    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent, PointLightComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, pointLight] = view.get<GamerEngine::TransformComponent, PointLightComponent>(entity);
                if (pointLight.GetLight())
                {
                    if (!pointLight.Active)
                    {
                        continue;
                    }

                    pointLight.GetLight()->SetData(&transform);
                    pointLight.OnUpdate();
                    RenderLight(pointLight.GetLight());
                }
            }
        }
    }

    {
        const auto& view = myRegistry.view<GamerEngine::TransformComponent, SpotLightComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, spotLight] = view.get<GamerEngine::TransformComponent, SpotLightComponent>(entity);
                if (spotLight.GetLight())
                {
                    if (!spotLight.Active)
                    {
                        continue;
                    }

                    spotLight.GetLight()->SetData(&transform);
                    spotLight.OnUpdate();
                    RenderLight(spotLight.GetLight());
                }
            }
        }
    }


    /*{
        const auto& view = myRegistry.view<TransformComponent, ModelComponent>();
        if (view != nullptr)
        {
            for (const auto& entity : view)
            {
                auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);

                if (model.GetModel())
                {
                    model.GetModel()->ClearInstanceData();
                    model.GetModel()->SetHasBeenRenderer(false);
                }
            }
        }
    }*/

    {
        //myLightToRender.push_back(myEnvironmentLight.get());
        for (const auto& light : myLightToRender)
        {
            light->Update();
        }
        CommonUtilities::MergeSort(myLightToRender);
    }
}

const std::string& GamerEngine::Scene::GetPath()
{
    return myPath;
}

Ref<DirectionalLight> GamerEngine::Scene::GetDirLight()
{
    return myDirectionalLight;
}

Ref<EnvironmentLight> GamerEngine::Scene::GetEnvLight()
{
    return myEnvironmentLight;
}

void GamerEngine::Scene::SceneReady(bool isSceneReady)
{
    mySceneIsReady = isSceneReady;
}

GamerEngine::Entity GamerEngine::Scene::GetEntityByUUID(UUID uuid)
{
    // TODO(Yan): Maybe should be assert
    if (myEntityMap.contains(uuid))
        return {myEntityMap.at(uuid), this };

    return {};
}

GamerEngine::Entity GamerEngine::Scene::FindEntityByName(std::string_view aName)
{
    auto view = myRegistry.view<TagComponent>();
    for (auto entity : view)
    {
        const TagComponent& tc = view.get<TagComponent>(entity);
        if (tc.Tag == aName)
            return Entity{ entity, this };
    }
    return {};
}

void GamerEngine::Scene::Clean()
{
    Light::Reset();
}

std::vector<Light*>& GamerEngine::Scene::GetLights()
{
    CommonUtilities::MergeSortPtr(myLightToRender);
    return myLightToRender;
}

void GamerEngine::Scene::RemoveLight(const Light* aLight)
{
    for (int i = 0; i < myLightToRender.size(); i++)
    {
        if (myLightToRender[i] == aLight)
        {
            myLightToRender.erase(myLightToRender.begin() + i);
            i--;
        }
    }
}

void GamerEngine::Scene::ResetLights()
{
    myLightToRender.clear();
}

void GamerEngine::Scene::RenderLight(Light* aLight)
{
    myLightToRender.push_back(aLight);
}

void GamerEngine::Scene::SetDirectionalLight(Ref<DirectionalLight> aDirectionalLight)
{
    myDirectionalLight = aDirectionalLight;
}

void GamerEngine::Scene::SetSceneStatus(SceneStatus aSceneStatus)
{
    mySceneStatus = aSceneStatus;
}

GamerEngine::SceneStatus GamerEngine::Scene::GetSceneStatus() const
{
    return mySceneStatus;
}

bool GamerEngine::Scene::IsReady() const
{
    return mySceneIsReady;
}

void GamerEngine::Scene::SetCameraHandle(const std::function<void(Entity)>& aCameraHandle)
{
    myEditorCamHandler = aCameraHandle;
}

void GamerEngine::Scene::SetPath(const std::string& aPath)
{
    myPath = aPath;
}

template <typename T>
void GamerEngine::Scene::OnComponentAdded(Entity entity, T& component)
{
    //static_assert(sizeof(T) == 0);
}

template <>
void GamerEngine::Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<GamerEngine::TransformComponent>(Entity entity, GamerEngine::TransformComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<GamerEngine::NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<GamerEngine::CameraComponent>(Entity entity, CameraComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<ParticleEmitter>(Entity entity, ParticleEmitter& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<DirectionalLightComponent>(Entity entity,
                                                                     DirectionalLightComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<SpotLightComponent>(Entity entity, SpotLightComponent& component)
{
}

template <>
void GamerEngine::Scene::OnComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component)
{
}
