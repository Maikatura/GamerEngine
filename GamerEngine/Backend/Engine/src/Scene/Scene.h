#pragma once
#include <memory>
#include "UUID.h"
#include <entt.hpp>
#include <Math/MathTypes.hpp>

#include "Light/Light.h"
#include "Sort/Sort.hpp"

#include <mutex>
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

	Entity CreateEntity(const std::string& aName);
	Entity CreateEntityWithUUID(const UUID2& aUUID, const std::string& aName);
	void DeleteEntity(Entity aEntity);

	virtual void OnUpdate(bool aShouldRunLoop = true, bool aLoadingScene = false);
	virtual void OnRender();

	template<typename T>
	void OnComponentAdded(Entity entity, T& component);

	void SetPath(const std::string& aPath);
	const std::string& GetPath();

	Ref<DirectionalLight> GetDirLight();
	Ref<EnvironmentLight> GetEnvLight();

	void SceneReady(bool isSceneReady);

	static void Clean();
	std::vector<Light*>& GetLights();
	void RemoveLight(const Light* aLight);
	void ResetLights();
	void RenderLight(Light* light);

	void SetDirectionalLight(Ref<DirectionalLight> aDirectionalLight);

	void SetSceneStatus(SceneStatus aSceneStatus);
	SceneStatus GetSceneStatus() const;
	bool IsReady() const;

	static void SetCameraHandle(const std::function<void(Entity)>& aCameraHandle);

private:
	SceneStatus mySceneStatus;
	bool mySceneIsReady = false;

	std::string myPath;
	entt::registry myRegistry;

	Ref<DirectionalLight> myDirectionalLight;
	Ref<EnvironmentLight> myEnvironmentLight;
	std::mutex myLock;

	//flecs::world myWorld;

	Vector4f mySceneBackgroundColor = {0.0f,0.0f,0.0f,1.0f};
	
	std::map<entt::id_type, std::string> myComponentMap;

	inline static std::function<void(Entity)> myEditorCamHandler;
	std::vector<Light*> myLightToRender;

	std::thread myStatisticThread;

	friend class Entity;
};


}


