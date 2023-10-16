#pragma once
#include <memory>
#include "UUID.h"
#include <entt.hpp>
#include <Math/MathTypes.hpp>

#include "Renderer/Light/Light.h"
#include "Sort/Sort.hpp"
#include <mutex>
//#include "flecs.h"

class ModelInstance;
class DirectionalLight;
class EnvironmentLight;
class SpotLight;
class PointLight;
class Entity;

namespace TurNet
{
	struct TurMessage;
}

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

	Entity CreateEntity(const std::string& aName);
	Entity CreateEntityWithUUID(const UUID2& aUUID, const std::string& aName);
	void DeleteEntity(Entity aEntity);

	virtual void OnUpdate(bool aShouldRunLoop = true, bool aLoadingScene = false);
	virtual void OnRender();

	template<typename T>
	void OnComponentAdded(Entity entity, T& component);

	void SetPath(const std::string& aPath);
	const std::string& GetPath();

	std::shared_ptr<DirectionalLight> GetDirLight();
	std::shared_ptr<EnvironmentLight> GetEnvLight();

	void SceneReady(bool isSceneReady);

	void Clean();
	std::vector<Light*> GetSomeLights();
	std::vector<Light*>& GetLights();
	void RemoveLight(Light* aLight);
	void ResetLights();
	void RenderLight(Light* light);

	Entity GetNetworkEntity(TurNet::TurMessage* aMessage);

	void SetSceneStatus(SceneStatus aSceneStatus);
	SceneStatus GetSceneStatus();
	bool IsReady();

	static void SetCameraHandle(std::function<void(Entity)> aCameraHandle);

private:
	SceneStatus mySceneStatus;
	bool mySceneIsReady = false;

	std::string myPath;
	entt::registry myRegistry;

	std::shared_ptr<DirectionalLight> myDirectionalLight;
	std::shared_ptr<EnvironmentLight> myEnvironmentLight;
	std::mutex myLock;

	//flecs::world myWorld;

	Vector4f mySceneBackgroundColor = {0.0f,0.0f,0.0f,1.0f};
	
	std::map<entt::id_type, std::string> myComponentMap;

	inline static std::function<void(Entity)> myEditorCamHandler;
	std::vector<Light*> myLightToRender;

	std::thread myStatisticThread;

	friend class Entity;
};


