#pragma once
#include <Math/MathTypes.hpp>
#include <string>
#include <vector>

class SceneObject;


class TransformOld
{
public:


	TransformOld() : mySceneObject(nullptr)
	{}

	TransformOld(SceneObject* aObject) : mySceneObject(aObject)
	{}

	TransformOld(Vector3f somePosition, Vector3f someRotation = Vector3f(), Vector3f someScale = Vector3f(1.0f, 1.0f, 1.0f));
	~TransformOld();
	Matrix4x4f& GetMatrix();
	Vector3f GetPosition();
	Vector3f GetRotation();
	Vector3f GetScale();
	Vector3f GetForward();
	Vector3f GetRight();
	Vector3f GetUp();
	void DecomposeTransform(Vector3f& somePosition, Vector3f& someRotation, Vector3f& someScale);
	void ComposeTransform(Vector3f somePosition, Vector3f someRotation, Vector3f someScale);
	void SetPosition(Vector3f aPosition);
	void SetRotation(Vector3f aRotation);
	void SetScale(Vector3f aScale);

	Matrix4x4f GetWorldMatrix();
	Vector3f GetWorldPosition();
	Vector3f GetWorldRotation();
	Vector3f GetWorldScale();

	void SetMatrix(Matrix4x4f aMatrix)
	{
		myTransformMatrix = aMatrix;
	}

	Vector3f GetXAxis()
	{
		auto myTransform = GetMatrix();
		return { myTransform(1, 1), myTransform(1, 2), myTransform(1, 3) };
	}

	Vector3f GetYAxis()
	{
		auto myTransform = GetMatrix();
		return {myTransform(2, 1), myTransform(2, 2), myTransform(2, 3) };
	}

	Vector3f GetZAxis()
	{
		auto myTransform = GetMatrix();
		return { myTransform(3, 1), myTransform(3, 2), myTransform(3, 3) };
	}

	void SetGameObject(SceneObject* aSceneObject);

	void SetParent(TransformOld* aTransform);
	void AddChild(TransformOld* aTransform);
	TransformOld* GetParent();
	bool GetHasParent();
	TransformOld* GetChild(int anIndex = 0);
	TransformOld* GetChild(std::string aName);
	bool GetHasChildren();

	

private:
	friend class SceneObject;
	TransformOld* myParent = nullptr;
	std::vector<TransformOld*> myChildren;
	Matrix4x4f myTransformMatrix;
	Vector3f myRotation;

	SceneObject* mySceneObject;

	std::string myName;


};
