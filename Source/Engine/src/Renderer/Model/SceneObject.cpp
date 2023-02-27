#include "GraphicsEngine.pch.h"
#include "SceneObject.h"

#include "Components/Components.hpp"


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

void SceneObject::SetPosition(Vector3f aPos)
{
	myTransform->SetPosition({ aPos.x, aPos.y, aPos.z });
}

void SceneObject::SetScale(float aX, float aY, float aZ)
{
	myTransform->SetScale({ aX, aY, aZ });
}

void SceneObject::SetTransform(Matrix4x4f aTransform)
{
	myTransform->SetMatrix(aTransform);
}

void SceneObject::Update()
{
	
}

Transform& SceneObject::GetTransform()
{ return *myTransform; }
