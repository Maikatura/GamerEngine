#pragma once
#include "GraphicsEngine.h"
#include "Model/Entity.h"
#include "Utilites/Pointers.h"

class TransformComponent : public Component
{
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };

public:

	SrdPtr<Entity> myParent;
	std::vector<SrdPtr<Entity>> myChildren{};


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

	void SetPosition(Vector3f aPosition)
	{
		Translation = aPosition;
	}

	Vector3f GetRotation() const
	{
		return Rotation;
	}

	Vector3f& GetRotation()
	{
		return Rotation;
	}

	void SetRotation(Vector3f aRotation)
	{
		Rotation = aRotation;
	}

	Vector3f GetScale() const
	{
		return Scale;
	}

	Vector3f& GetScale()
	{
		return Scale;
	}

	void SetScale(Vector3f aScale)
	{
		Scale = aScale;
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
		const float forwardDistance = 5.0f;
		Vector3f forwardVector = CommonUtilities::Quat(Rotation).Forward();
		return Translation + forwardVector * forwardDistance;
	}

	Matrix4x4f GetMatrix()
	{
		return ComposeFromTRS(Translation, CommonUtilities::Quat::FromEulers(ToRadians(Rotation)), Scale);
	}

	void SetChild(SrdPtr<Entity> aChild)
	{
		myChildren.push_back(aChild);
	}

	void RemoveChild(SrdPtr<Entity> aChild)
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

	void SetParent(SrdPtr<Entity> aParent)
	{
		myParent = aParent;
	}

	void ClearParent()
	{
		myParent = MakeSrdPtr<Entity>();
	}

	SrdPtr<Entity> GetParent() const
	{
		return myParent;
	}

	std::vector<SrdPtr<Entity>> GetChildren() const
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
};

