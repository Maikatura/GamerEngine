#pragma once
class Entity;

class Component
{
public:

	Component(bool aRunInEditor = false);

	bool RunInEditor();

	void SetEntity(Entity* aEntity);

protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
	virtual void OnRender() {}

	bool myRunInEditor = false;
	Entity* myEntity;
};
