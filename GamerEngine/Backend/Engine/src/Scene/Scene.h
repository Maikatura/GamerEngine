#pragma once
#include <memory>
#include "UUID.h"
#include <entt.hpp>
#include <Math/MathTypes.hpp>
#include <unordered_map>

#include "Light/Light.h"
#include "Sort/Sort.hpp"

#include <mutex>

#include "Core/Rendering/EditorCamera.h"
//#include "flecs.h"

class DirectionalLight;
class EnvironmentLight;
class SpotLight;
class PointLight;

namespace TurNet
{
    struct TurMessage;
}

namespace GamerEngine
{
    class Model;
    class Entity;


    enum class SceneStatus
    {
        None,
        NeedSwap,
        Loading,
        Complete,
        Count
    };

    class Scene
    {
    public:
        Scene();
        virtual ~Scene();

        bool Initialize();

        void Clear();

        void Resize(Vector2ui aNewWindowSize);
        entt::registry& GetRegistry();

        Entity CreateEntity(const std::string& aName = std::string());
        Entity CreateEntityWithUUID(UUID aUUID, const std::string& aName = std::string());
        void DeleteEntity(Entity aEntity);


    	void OnRuntimeStart();
    	void OnRuntimeStop();
        virtual void OnUpdate(bool aShouldRunLoop = true, bool aLoadingScene = false);
        virtual void OnRender();

        template <typename T>
        void OnComponentAdded(Entity entity, T& component);

        void SetPath(const std::string& aPath);
        const std::string& GetPath();

        Ref<DirectionalLight> GetDirLight();
        Ref<EnvironmentLight> GetEnvLight();

        void SceneReady(bool isSceneReady);
        Entity GetEntityByUUID(UUID uuid);
        Entity FindEntityByName(std::string_view aName);

        static void Clean();
        std::vector<Light*>& GetLights();
        void RemoveLight(const Light* aLight);
        void ResetLights();
        void RenderLight(Light* light);

        void SetDirectionalLight(Ref<DirectionalLight> aDirectionalLight);

        void SetSceneStatus(SceneStatus aSceneStatus);
        SceneStatus GetSceneStatus() const;
        bool IsReady() const;

        template <typename... Components>
        auto GetAllEntitiesWith()
        {
            return myRegistry.view<Components...>();
        }

        static void SetCameraHandle(const std::function<void(Entity)>& aCameraHandle);

    private:
        SceneStatus mySceneStatus;
        bool mySceneIsReady = false;

        std::string myPath;
        entt::registry myRegistry;

        EditorCamera myEditorCamera;

        Ref<DirectionalLight> myDirectionalLight;
        Ref<EnvironmentLight> myEnvironmentLight;
        std::mutex myLock;

        //flecs::world myWorld;

        Vector4f mySceneBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};


        std::unordered_map<GamerEngine::UUID, entt::entity> myEntityMap;

        std::map<entt::id_type, std::string> myComponentMap;

        inline static std::function<void(Entity)> myEditorCamHandler;
        std::vector<Light*> myLightToRender;

        std::thread myStatisticThread;

        friend class Entity;
    };
}
