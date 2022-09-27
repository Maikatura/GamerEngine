#include "GraphicsEngine.pch.h"
#include "SceneObject.h"


void SceneObject::SetTransform(Transform aTransform)
{
	myTransform = std::make_shared<Transform>(aTransform);
}

void SceneObject::SetRotation(float aPitch, float aYaw, float aRoll)
{
	myTransform->SetRotation({ aPitch, aYaw, aRoll});
}

void SceneObject::SetPosition(float aX, float aY, float aZ)
{
	myTransform->SetPosition({ aX, aY, aZ });
}

void SceneObject::SetScale(float aX, float aY, float aZ)
{
	myTransform->SetScale({ aX, aY, aZ });
}

void SceneObject::Update()
{
	
}
