#pragma once
#include "Transform.h"

class SceneObject
{
protected:
	Transform myTransform;

public:
	SceneObject() = default;
	virtual ~SceneObject() = default;

	void SetTrasnform(Transform aTransform);
	void SetRotation(float aPitch, float aYaw, float aRoll);
	void SetPosition(float aX, float aY, float aZ);
	void SetScale(float aX, float aY, float aZ);

	FORCEINLINE Transform& GetTransform() { return myTransform; }
};
