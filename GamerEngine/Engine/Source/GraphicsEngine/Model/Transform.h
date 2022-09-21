#pragma once
#include <Math/MathTypes.hpp>
#include <string>
#include <vector>

class SceneObject;


class Transform
{
public:
	Transform() = default;
	Transform(Vector3f somePosition, Vector3f someRotation = Vector3f(), Vector3f someScale = Vector3f(1.0f, 1.0f, 1.0f));
	~Transform() = default;
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

	void SetGameObject(SceneObject* aSceneObject);

	void SetParent(Transform* aTransform);
	void AddChild(Transform* aTransform);
	Transform* GetParent();
	bool GetHasParent();
	Transform* GetChild(int anIndex = 0);
	Transform* GetChild(std::string aName);
	bool GetHasChildren();

	

private:
	friend class SceneObject;
	Transform* myParent = nullptr;
	std::vector<Transform*> myChildren;
	Matrix4x4f myTransformMatrix;
	Vector3f myRotation;

	SceneObject* mySceneObject;

	std::string myName;


};
