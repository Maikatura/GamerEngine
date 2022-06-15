#include "GraphicsEngine.pch.h"
#include "SceneObject.h"

void SceneObject::SetTrasnform(Transform aTransform)
{
	myTransform = aTransform;
}

void SceneObject::SetRotation(float aPitch, float aYaw, float aRoll)
{
}

void SceneObject::SetPosition(float aX, float aY, float aZ)
{
	myTransform.SetPosition({ aX, aY, aZ, 1.0f });
}
