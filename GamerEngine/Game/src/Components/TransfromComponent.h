#pragma once
#include "Model/Entity.h"
#include "Utilites/Pointers.h"

class TransformComponent : public Component
{
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };

	Matrix4x4f Matrix;

public:

	Ref<Entity> myParent;
	std::vector<Ref<Entity>> myChildren{};


	uint64_t TempParent;
	std::vector<uint64_t> TempChildren;


	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Vector3f& translation)
		: Translation(translation)
	{}


	Vector3f GetPosition() const
	{
		return Translation;
	}

	Vector3f& GetPosition()
	{
		return Translation;
	}

	Vector3f GetRotation() const
	{
		return Rotation;
	}

	Vector3f& GetRotation()
	{
		return Rotation;
	}

	Vector3f GetScale() const
	{
		return Scale;
	}

	Vector3f& GetScale()
	{
		return Scale;
	}

	void SetPosition(Vector3f aPosition)
	{
		Translation = aPosition;
	}

	void SetRotation(Vector3f aRotation)
	{
		Rotation = aRotation;
	}

	void SetScale(Vector3f aScale)
	{
		Scale = aScale;
	}

	Matrix4x4f GetMatrix() const
	{
		return ComposeFromTRS(Translation, Vector3f(Rotation.x, Rotation.y, Rotation.z), Scale);
	}

	Vector3f GetForward()
	{
		return GetMatrix().GetForward();
	}

	Vector3f GetRight()
	{
		return GetMatrix().GetRight();
	}

	Vector3f GetUp()
	{
		return GetMatrix().GetUp();
	}



	void LookAt(const Vector3f aPosition)
	{
		if (aPosition.x == Translation.x &&
			aPosition.y == Translation.y &&
			aPosition.z == Translation.z)
		{
			return;
		}

		Vector3f lookAtPos;
		lookAtPos.x = Translation.x - aPosition.x;
		lookAtPos.y = Translation.y - aPosition.y;
		lookAtPos.z = Translation.z - aPosition.z;

		float pitch = 0.0f;
		if (lookAtPos.y != 0.0f)
		{
			const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
			pitch = atan(lookAtPos.y / distance);
		}

		float yaw = 0.0f;
		if (lookAtPos.x != 0.0f)
		{
			yaw = atan(lookAtPos.x / lookAtPos.z);
		}

		if (lookAtPos.z > 0.0f)
		{
			yaw += 3.1415f;
		}


		Rotation = { pitch * (180.0f / 3.14f), yaw * (180.0f / 3.14f), 0.0f };
	}

	Vector3f Forward()
	{
		/*const float forwardDistance = 5.0f;
		Vector3f forwardVector = Quatf(Rotation).Forward();
		return Translation + forwardVector * forwardDistance;*/

		// TODO : FIX THIS FORWARD IN TRANSFORMCOMPONENT

		return Vector3f();
	}

	

	void SetChild(Ref<Entity> aChild)
	{
		myChildren.push_back(aChild);
	}

	void RemoveChild(Ref<Entity> aChild)
	{
		for (size_t i = 0; i < myChildren.size(); i++)
		{
			if (myChildren[i] == aChild)
			{
				myChildren.erase(myChildren.begin() + i);
				i--;
			}
		}
	}

	void SetParent(Ref<Entity> aParent)
	{
		myParent = aParent;
	}

	void ClearParent()
	{
		myParent = MakeRef<Entity>();
	}

	Ref<Entity> GetParent() const
	{
		return myParent;
	}

	std::vector<Ref<Entity>> GetChildren() const
	{
		return myChildren;
	}

	bool HasParent()
	{
		if (!myParent)
		{
			return false;
		}

		return (myParent->GetHandle() != entt::null);
	}


	void UpdateTransformFromMatrix()
	{
		/*Translation = Matrix.GetPosition();
		Rotation = Matrix.GetRotation().Eulers();
		Scale = Matrix.GetScale();*/
	}

	void BuildTransform()
	{
		/*CommonUtilities::Quat rotationQuaternion = CommonUtilities::Quat::FromEulers(ToRadians(Vector3f(Rotation.x, Rotation.y, Rotation.z)));
		Matrix = ComposeFromTRS(Translation, rotationQuaternion, Scale);*/
	}

	GraphicsTransform AsGraphicsTransform()
	{
		GraphicsTransform transform;
		//(GetMatrix(), transform.Translation, transform.Rotation, transform.Scale);
		return transform;
	}

private:

};

