#pragma once
class Entity;

class Component
{
public:
	virtual ~Component() = default;

	Component(bool aRunInEditor = false);

	bool RunInEditor();

	void SetEntity(Entity* aEntity);
	Entity* GetEntity() { return myEntity; }

protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
	virtual void OnRender() {}

	bool myRunInEditor = false;
	Entity* myEntity;
};
