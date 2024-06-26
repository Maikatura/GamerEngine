#pragma once
#include <entt.hpp>
#include "Camera.h"
#include "Core/Model/Entity.h"
#include "Scene/SceneManager.h"

namespace GamerEngine
{


	struct Transform
	{
		Vector3f Translation = { 0.0f, 0.0f, 0.0f };
		Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3f Scale = { 1.0f, 1.0f, 1.0f };

		Matrix4x4f GetMatrix() const
		{
			return ComposeFromTRS(Translation, Rotation, Scale);
		}
	};

	class TransformComponent
	{
		

	public:

		Vector3f Translation = { 0.0f, 0.0f, 0.0f };
		Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3f Scale = { 1.0f, 1.0f, 1.0f };

		uint64_t myParent = 0;
		std::vector<uint64_t> myChildren{};



		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vector3f& translation)
			: Translation(translation)
		{}
		TransformComponent(const Transform& aTransform)
			: Translation(aTransform.Translation), Rotation(aTransform.Rotation), Scale(aTransform.Scale)
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




		Matrix4x4f GetLocalMatrix() const
		{
			return ComposeFromTRS(Translation, Vector3f(Rotation.x, Rotation.y, Rotation.z), Scale);
		}

		Matrix4x4f GetWorldMatrix() const
		{
			const auto transform = GetWorldTransform();
			return ComposeFromTRS(transform.Translation, transform.Rotation, transform.Scale);
		}

		Transform GetTransform() const
		{
			return { Translation, Rotation, Scale };
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
				auto parentEntity = Entity(SceneManager::Get().GetScene()->GetEntityByUUID(myParent), SceneManager::Get().GetScene().get());
				TransformComponent& parentTransform = parentEntity.GetComponent<GamerEngine::TransformComponent>();
				Matrix4x4f parentWorldMatrix = parentTransform.GetWorldMatrix();


				// Calculate child's translation relative to parent's forward and right directions
				transform.Translation = parentWorldMatrix.GetForward() * Translation.z +
					parentWorldMatrix.GetRight() * Translation.x +
					parentWorldMatrix.GetUp() * Translation.y +
					parentWorldMatrix.GetPosition();
				transform.Rotation += parentTransform.GetWorldTransform().Rotation;
				transform.Scale *= parentTransform.GetWorldTransform().Scale;

			}

			return transform;
		}

		Vector3f GetForward() const
		{
			return GetLocalMatrix().GetForward();
		}

		Vector3f GetRight() const
		{
			return GetLocalMatrix().GetRight();
		}

		Vector3f GetUp() const
		{
			return GetLocalMatrix().GetUp();
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
			myChildren.push_back(aChild.GetUUID());
		}

		void RemoveChild(GamerEngine::Entity aChild)
		{
			for (size_t i = 0; i < myChildren.size(); i++)
			{
				if (myChildren[i] == aChild.GetUUID())
				{
					myChildren.erase(myChildren.begin() + i);
					i--;
				}
			}
		}

		void SetParent(GamerEngine::Entity aParent)
		{
			myParent = aParent.GetUUID();
		}

		void ClearParent()
		{
			myParent = 0;
		}

		uint64_t GetParent()
		{
			return myParent;
		}

		std::vector<uint64_t> GetChildren() const;

		void ClearChildren()
		{
			myChildren.clear();
		}

		bool HasParent() const;


		void UpdateTransformFromMatrix()
		{
			/*Translation = Matrix.GetMousePosition();
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


		bool HasChanged(Vector3f aPosition)
		{
			return Translation.Compare(aPosition);
		}
	};

}

