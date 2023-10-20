#include "GraphicsEngine.pch.h"
#include <Model/Transform.h>
#include <Model/SceneObject.h>


Transform::Transform(Vector3f somePosition, Vector3f someRotation, Vector3f someScale)
{
	Vector3f shear = Vector3f(0, 0, 0);

	ComposeTransform(somePosition, someRotation, someScale);
}

Transform::~Transform()
{
}

Matrix4x4f& Transform::GetMatrix()
{
	return myTransformMatrix;
}

Vector3f Transform::GetPosition()
{
	return { myTransformMatrix(4,1), myTransformMatrix(4,2), myTransformMatrix(4,3) };
}

Vector3f Transform::GetRotation()
{
	return myRotation;
}

Vector3f Transform::GetScale()
{
	Vector3f output;
	
	Vector4f scaleX = { myTransformMatrix(1,1), myTransformMatrix(1,2), myTransformMatrix(1,3), myTransformMatrix(1,4) };
	Vector4f scaleY = { myTransformMatrix(2,1), myTransformMatrix(2,2), myTransformMatrix(2,3), myTransformMatrix(2,4) };
	Vector4f scaleZ = { myTransformMatrix(3,1), myTransformMatrix(3,2), myTransformMatrix(3,3), myTransformMatrix(3,4) };

	output.x = scaleX.Length();
	output.y = scaleY.Length();
	output.z = scaleZ.Length();

	return output;
}

Vector3f Transform::GetForward()
{
	return { myTransformMatrix(3,1), myTransformMatrix(3,2), myTransformMatrix(3,3) };
}

Vector3f Transform::GetRight()
{
	return { myTransformMatrix(1,1), myTransformMatrix(1,2), myTransformMatrix(1,3) };
}

Vector3f Transform::GetUp()
{
	return { myTransformMatrix(2,1), myTransformMatrix(2,2), myTransformMatrix(2,3) };
}

void Transform::DecomposeTransform(Vector3f& somePosition, Vector3f& someRotation, Vector3f& someScale)
{
	somePosition = GetPosition();
	someRotation = myRotation;
	someScale = GetScale();
}

void Transform::ComposeTransform(Vector3f somePosition, Vector3f someRotation, Vector3f someScale)
{
	//myRotation = someRotation;

	// Create translation matrix
	Matrix4x4f translationMatrix = Matrix4x4f::CreateTranslation(somePosition);

	// Create rotation matrix (order of rotations: pitch, yaw, roll)
	Matrix4x4f rotationMatrix = Matrix4x4f::CreateRotationAroundX(someRotation.x) *
		Matrix4x4f::CreateRotationAroundY(someRotation.y) *
		Matrix4x4f::CreateRotationAroundZ(someRotation.z);

	// Create scale matrix
	Matrix4x4f scaleMatrix = Matrix4x4f::CreateScale(someScale);

	// Combine the matrices in SRT order (scale, then rotate, then translate)
	Matrix4x4f transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	
	myTransformMatrix = transformMatrix;
}

void Transform::SetPosition(Vector3f aPosition)
{
	myTransformMatrix(4, 1) = aPosition.x;
	myTransformMatrix(4, 2) = aPosition.y;
	myTransformMatrix(4, 3) = aPosition.z;
	myTransformMatrix(4, 4) = 1;
}

void Transform::SetRotation(Vector3f aRotation)
{
	myRotation = aRotation;

	Vector3f pos = GetPosition();
	Vector3f scale = GetScale();

	ComposeTransform(pos, aRotation, scale);
}

void Transform::SetScale(Vector3f aScale)
{
	Vector3f pos = GetPosition();
	Vector3f rot = myRotation;


	ComposeTransform(pos, rot, aScale);
}

Matrix4x4f Transform::GetWorldMatrix()
{
	Matrix4x4f output = GetMatrix();
	Transform* parent = myParent;
	while(parent != nullptr)
	{
		output *= parent->GetMatrix();
		parent = parent->GetParent();
	}
	return output;
}

Vector3f Transform::GetWorldPosition()
{
	Matrix4x4f matr = GetWorldMatrix();
	Vector3f output;
	Vector3f useless;
	//matr.DecomposeAsTransform(output, useless, useless, useless);
	return output;
}

Vector3f Transform::GetWorldRotation()
{
	Matrix4x4f matr = GetWorldMatrix();
	Vector3f output;
	Vector3f useless;
	//matr.DecomposeAsTransform(useless, output, useless, useless);
	return output;
}

Vector3f Transform::GetWorldScale()
{
	Matrix4x4f matr = GetWorldMatrix();
	Vector3f output;
	Vector3f useless;
	//matr.DecomposeAsTransform(useless, useless, output, useless);
	return output;
}

void Transform::SetGameObject(SceneObject* aSceneObject)
{
	mySceneObject = aSceneObject;
}

void Transform::SetParent(Transform* aTransform)
{
	myParent = aTransform;
	aTransform->myChildren.push_back(this);
}

void Transform::AddChild(Transform* aTransform)
{
	myChildren.push_back(aTransform);
	aTransform->myParent = this;
}

Transform* Transform::GetParent()
{
	return myParent;
}

bool Transform::GetHasParent()
{
	return myParent != nullptr;
}

Transform* Transform::GetChild(int anIndex)
{
	return myChildren.size() > anIndex ? myChildren[anIndex] : nullptr;
}

Transform* Transform::GetChild(std::string aName)
{
	for(Transform* t : myChildren)
	{
		/*if(t->->Name() == aName)
		{
			return t;
		}*/
	}
	return nullptr;
}

bool Transform::GetHasChildren()
{
	return myChildren.size() != 0;
}
