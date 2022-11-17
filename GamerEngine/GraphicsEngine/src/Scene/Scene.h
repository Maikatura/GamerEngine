#pragma once
#include <memory>
#include "UUID.h"
#include <entt/entt.hpp>
#include <Math/MathTypes.hpp>


class ModelInstance;
class DirectionalLight;
class EnvironmentLight;
class SpotLight;
class PointLight;
class Entity;

class Scene
{
public:

	Scene() = default;
	~Scene();

	bool Initialize();

	void Clear();

	void Resize(Vector2ui aNewWindowSize);
	entt::registry& GetRegistry();

	Entity CreateEntity(const std::string& aName);
	Entity CreateEntityWithUUID(UUID2 aUUID, const std::string& aName);
	void DeleteEntity(Entity aEntity);

	virtual void OnUpdate(bool aShouldRunLoop = true);
	virtual void OnRender();

	template<typename T>
	void OnComponentAdded(Entity entity, T& component);

	void SetPath(const std::string& aPath);
	const std::string& GetPath();

	std::shared_ptr<DirectionalLight> GetDirLight();
	std::shared_ptr<EnvironmentLight> GetEnvLight();
	void Clean();

private:
	std::string myPath;
	entt::registry myRegistry;

	std::shared_ptr<DirectionalLight> myDirectionalLight;
	std::shared_ptr<EnvironmentLight> myEnvironmentLight;

	Vector4f mySceneBackgroundColor = {0.0f,0.0f,0.0f,1.0f};

	std::map<entt::id_type, std::string> myComponentMap;

	friend class Entity;
};


