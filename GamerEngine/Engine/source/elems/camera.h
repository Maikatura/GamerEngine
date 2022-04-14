#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "input.h"
#include "Components/MeshComponent.h"
#include "shader/shader_util.h"

namespace GamerEngine
{
	class Camera
	{
	public:

		Camera(const glm::vec3& position, float fov, float aspect, float near, float far)
		{
			mPosition = position;
			mAspect = aspect;
			mNear = near;
			mFar = far;
			mFOV = fov;

			SetAspect(mAspect);

			update_view_matrix();
		}


		void Update(MeshComponent* aModel, nshaders::Shader* shader)
		{
			if (aModel)
			{
				glm::mat4 position(1.0f);
				glm::mat4 rotation(1.0f);
				glm::mat4 scale(1.0f);
				position = glm::translate(position, aModel->myModel.myTransform.myPosition);
				rotation = glm::rotate(rotation, glm::radians(aModel->myModel.myTransform.myRotation.x), glm::vec3(1, 0, 0));
				rotation = glm::rotate(rotation, glm::radians(aModel->myModel.myTransform.myRotation.y), glm::vec3(0, 1, 0));
				rotation = glm::rotate(rotation, glm::radians(aModel->myModel.myTransform.myRotation.z), glm::vec3(0, 0, 1));
				scale = glm::scale(scale, aModel->myModel.myTransform.myScale);

				glm::mat4 modelView = position * scale * rotation;

				shader->set_mat4(modelView, "model");
			}
			else
			{
				glm::mat4 model(1.0f);
				shader->set_mat4(model, "model");
			}
			shader->set_mat4(mViewMatrix, "view");
			shader->set_mat4(GetProjection(), "projection");
			shader->set_vec3(mPosition, "camPos");

		}

		void SetPosition(glm::vec3 aPosition)
		{
			mPosition += -GetRight() * aPosition.x;
			mPosition += GetUp() * aPosition.y;
			mPosition += GetForward() * aPosition.z;
		}

		void SetAspect(float aspect)
		{
			mProjection = glm::perspective(mFOV, aspect, mNear, mFar);
		}

		void SetDistance(float offset)
		{
			mDistance += offset;
			update_view_matrix();
		}

		const glm::mat4& GetProjection() const
		{
			return mProjection;
		}

		glm::mat4 GetViewProjection() const
		{
			return mProjection * GetViewMatrix();
		}

		glm::vec3 GetUp() const
		{
			return glm::rotate(GetDirection(), cUp);
		}

		glm::vec3 GetRight() const
		{
			return glm::rotate(GetDirection(), cRight);
		}

		glm::vec3 GetForward() const
		{
			return glm::rotate(GetDirection(), cForward);
		}

		glm::quat GetDirection() const
		{
			return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
		}

		glm::mat4 GetViewMatrix() const
		{
			return mViewMatrix;
		}

		void OnMouseWheel(double delta)
		{
			SetDistance(delta * 0.5f);

			update_view_matrix();
		}

		void Reset()
		{
			mFocus = { 0.0f, 0.0f, 0.0f };
			//mDistance = 5.0f;
			update_view_matrix();
		}

		void OnMouseMove(double x, double y, EInputButton button)
		{
			glm::vec2 pos2d{ x, y };

			if (button == EInputButton::Right)
			{
				glm::vec2 delta = (pos2d - mCurrentPos2d) * 0.004f;

				float sign = GetUp().y < 0 ? -1.0f : 1.0f;
				mYaw += sign * delta.x * cRotationSpeed;
				mPitch += delta.y * cRotationSpeed;

				update_view_matrix();
			}
			else if (button == EInputButton::Middle)
			{
				// TODO: Adjust pan speed for distance
				glm::vec2 delta = (pos2d - mCurrentPos2d) * 0.003f;

				mFocus += -GetRight() * delta.x * mDistance;
				mFocus += GetUp() * delta.y * mDistance;

				update_view_matrix();
			}


			mCurrentPos2d = pos2d;
		}

		void update_view_matrix()
		{
			//mPosition = mFocus - GetForward();

			glm::quat orientation = GetDirection();
			mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
			mViewMatrix = glm::inverse(mViewMatrix);
		}

	private:
		glm::mat4 mViewMatrix;
		glm::mat4 mProjection = glm::mat4{ 1.0f };
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };

		glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };

		float mDistance = 5.0f;
		float mAspect;
		float mFOV;
		float mNear;
		float mFar;

		float mPitch = 0.0f;
		float mYaw = 0.0f;

		glm::vec2 mCurrentPos2d = { 0.0f, 0.0f };

		const glm::vec3 cRight = { 1.0f, 0.0f, 0.0f };
		const glm::vec3 cUp = { 0.0f, 1.0f, 0.0f };
		const glm::vec3 cForward = { 0.0f, 0.0f, -1.0f };

		const float cRotationSpeed = 2.0f;

	};
}

