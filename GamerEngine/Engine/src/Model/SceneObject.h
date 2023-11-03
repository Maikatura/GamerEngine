#pragma once
#include "Model/Transform.h"
#include "Utilites/Pointers.h"

class TransformComponent;


class SceneObject
{
protected:
	Ref<Transform> myTransform;



public:


	std::string Name;
	SceneObject()
	{
		myTransform = MakeRef<Transform>(this);
		myTransform->SetGameObject(this);
	}
	virtual ~SceneObject() = default;

	void SetTransform(Transform aTransform);
	void SetRotation(float aPitch, float aYaw, float aRoll);
	void SetPosition(float aX, float aY, float aZ);
	void SetPosition(Vector3f aPos);
	void SetScale(float aX, float aY, float aZ);


	void SetTransform(Matrix4x4f aTransform);

	virtual void Update();


	Transform& GetTransform();
};
