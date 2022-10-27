#pragma once
#include "Model/Transform.h"


class SceneObject
{
protected:
	std::shared_ptr<Transform> myTransform;

public:


	std::string Name;
	SceneObject()
	{
		myTransform = std::make_shared<Transform>(this);
		myTransform->SetGameObject(this);
	}
	virtual ~SceneObject() = default;

	void SetTransform(Transform aTransform);
	void SetRotation(float aPitch, float aYaw, float aRoll);
	void SetPosition(float aX, float aY, float aZ);
	void SetScale(float aX, float aY, float aZ);

	virtual void Update();


	Transform& GetTransform();
};
