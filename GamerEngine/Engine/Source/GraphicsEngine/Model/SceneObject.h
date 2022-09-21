#pragma once
#include "Transform.h"


class SceneObject
{
protected:
	std::shared_ptr<Transform> myTransform;

public:


	std::string Name;
	SceneObject() { myTransform = std::make_shared<Transform>(); myTransform->SetGameObject(this); }
	~SceneObject() = default;

	void SetTransform(Transform aTransform);
	void SetRotation(float aPitch, float aYaw, float aRoll);
	void SetPosition(float aX, float aY, float aZ);
	void SetScale(float aX, float aY, float aZ);

	virtual void Update();


	


	FORCEINLINE Transform& GetTransform() { return *myTransform; }
};
