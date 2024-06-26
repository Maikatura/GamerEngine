#pragma once
#include "PhysicsTypes.h"

namespace GamerEngine {

	class CharacterController : public RefCounted
	{
	public:
		virtual ~CharacterController() = default;

		virtual void SetGravityEnabled(bool enableGravity) = 0;
		virtual bool IsGravityEnabled() const = 0;

		virtual void SetSlopeLimit(float slopeLimit) = 0;
		virtual void SetStepOffset(float stepOffset) = 0;

		virtual void SetTranslation(const Vector3f& inTranslation) = 0;
		//virtual void SetRotation(const glm::quat& inRotation) = 0;

		virtual bool IsGrounded() const = 0;

		virtual ECollisionFlags GetCollisionFlags() const = 0;

		virtual void Move(const Vector3f& displacement) = 0;
		virtual void Jump(float jumpPower) = 0;

		virtual Vector3f GetLinearVelocity() const = 0;
		virtual void SetLinearVelocity(const Vector3f& inVelocity) = 0;

	private:
		virtual void PreSimulate(float deltaTime) {}
		virtual void Simulate(float deltaTime) = 0;

	private:
		friend class PhysicsScene;
	};

}
