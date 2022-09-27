#pragma once
#include <vector>
#include <memory>
#include <Model/ModelInstance.h>
#include <Model/EnumTypes.h>
#include <model/SelectedObject.hpp>
#include <entt/entt.hpp>

#include "UUID.h"
#include "Components/Components.hpp"

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

	virtual void OnUpdate();
	virtual void OnRender();

	template<typename T>
	void OnComponentAdded(Entity entity, T& component);

	void SetPath(const std::string& aPath);
	const std::string& GetPath();

private:
	std::string myPath;
	entt::registry myRegistry;

	friend class Entity;
};


