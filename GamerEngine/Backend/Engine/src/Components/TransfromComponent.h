#pragma once
#include <entt.hpp>
#include "Camera.h"
#include "Core/Model/Entity.h"


struct Transform
{
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };
};

class TransformComponent
{
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };

public:

	GamerEngine::Entity myParent;
	std::vector<GamerEngine::Entity> myChildren{};



	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Vector3f& translation)
		: Translation(translation)
	{}


	Vector3f GetPosition() const
	{
		return Translation;
	}

	Vector3f GetRotation() const
	{
		return Rotation;
	}

	Vector3f GetScale() const
	{
		return Scale;
	}


	void SetPosition(Vector3f aPosition)
	{
		if (HasParent())
		{
			auto& parentTransform = GetParent().GetComponent<TransformComponent>();
			Translation = aPosition - parentTransform.GetWorldTransform().Translation;
		}
		else
		{
			Translation = aPosition;
		}
	}

	void SetRotation(Vector3f aRotation)
	{
		if (HasParent())
		{
			auto& parentTransform = GetParent().GetComponent<TransformComponent>();
			Rotation = aRotation - parentTransform.GetWorldTransform().Rotation;
		}
		else
		{
			Rotation = aRotation;
		}
	}

	void SetScale(Vector3f aScale)
	{
		if (HasParent())
		{
			auto& parentTransform = GetParent().GetComponent<TransformComponent>();
			Scale = aScale * parentTransform.GetWorldTransform().Scale;
		}
		else
		{
			Scale = aScale;
		}
	}


	Matrix4x4f GetLocalMatrix() const
	{
		return ComposeFromTRS(Translation, Vector3f(Rotation.x, Rotation.y, Rotation.z), Scale);
	}

	Matrix4x4f GetWorldMatrix() const
	{
		Vector3f translation = Translation;
		Vector3f rot = Rotation;
		Vector3f scale = Scale;

		if (HasParent())
		{
			// Get parent's world transformation matrix
			const auto parentTransform = GetWorldTransform();

			// Calculate child's translation relative to parent's forward and right directions
			translation = parentTransform.Translation;
			rot = parentTransform.Rotation;
			scale = parentTransform.Scale;
		}

		return ComposeFromTRS(translation, Vector3f(rot.x, rot.y, rot.z), scale);
	}

	Transform GetWorldTransform() const
	{
		Transform transform;

		transform.Translation = Translation;
		transform.Rotation = Rotation;
		transform.Scale = Scale;

		if (HasParent())
		{
			// Get parent's world transformation matrix
			const auto& parentTransform = GetParent().GetComponent<TransformComponent>();

			// Calculate child's translation relative to parent's forward and right directions
			transform.Translation = parentTransform.GetForward() * Translation.z +
				parentTransform.GetRight() * Translation.x +
				parentTransform.GetUp() * Translation.y +
				parentTransform.GetPosition();
			transform.Rotation += parentTransform.GetRotation();
			transform.Scale *= parentTransform.GetScale();
		}

		return transform;
	}

	Vector3f GetForward() const
	{
		return GetWorldMatrix().GetForward();
	}

	Vector3f GetRight() const
	{
		return GetWorldMatrix().GetRight();
	}

	Vector3f GetUp() const
	{
		return GetWorldMatrix().GetUp();
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



	void SetChild(GamerEngine::Entity aChild)
	{
		myChildren.push_back(aChild);
	}

	void RemoveChild(GamerEngine::Entity aChild)
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

	void SetParent(GamerEngine::Entity aParent)
	{
		myParent = aParent;
	}

	void ClearParent()
	{
		myParent;
	}

	GamerEngine::Entity GetParent() const
	{
		return myParent;
	}

	std::vector<GamerEngine::Entity> GetChildren() const;

	bool HasParent() const;


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

