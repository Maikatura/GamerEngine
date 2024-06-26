#pragma once

#include "Physics/PhysicsBody.h"

#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Constraints/SixDOFConstraint.h>

#include "Core/Model/Entity.h"

namespace GamerEngine {

	class JoltBody : public PhysicsBody
	{
	public:
		JoltBody(JPH::BodyInterface& bodyInterface, Entity entity);
		~JoltBody();

		virtual void SetCollisionLayer(uint32_t layerID) override;

		virtual bool IsStatic() const override;
		virtual bool IsDynamic() const override;
		virtual bool IsKinematic() const override;

		virtual void SetGravityEnabled(bool isEnabled) override;

		virtual void AddForce(const Vector3f& force, EForceMode forceMode = EForceMode::Force, bool forceWake = true) override;
		virtual void AddForce(const Vector3f& force, const Vector3f& location, EForceMode forceMode = EForceMode::Force, bool forceWake = true) override;
		virtual void AddTorque(const Vector3f& torque, bool forceWake = true) override;

		virtual void ChangeTriggerState(bool isTrigger) override;
		virtual bool IsTrigger() const override;

		virtual float GetMass() const override;
		virtual void SetMass(float mass) override;

		virtual void SetLinearDrag(float inLinearDrag) override;
		virtual void SetAngularDrag(float inAngularDrag) override;

		virtual Vector3f GetLinearVelocity() const override;
		virtual void SetLinearVelocity(const Vector3f& inVelocity) override;

		virtual Vector3f GetAngularVelocity() const override;
		virtual void SetAngularVelocity(const Vector3f& inVelocity) override;

		virtual float GetMaxLinearVelocity() const override;
		virtual void SetMaxLinearVelocity(float inMaxVelocity) override;

		virtual float GetMaxAngularVelocity() const override;
		virtual void SetMaxAngularVelocity(float inMaxVelocity) override;

		virtual bool IsSleeping() const override;
		virtual void SetSleepState(bool inSleep) override;

		virtual void AddRadialImpulse(const Vector3f& origin, float radius, float strength, EFalloffMode falloff, bool velocityChange) override;

		virtual void SetCollisionDetectionMode(ECollisionDetectionType collisionDetectionMode) override;

		JPH::BodyID GetBodyID() const { return m_BodyID; }

		virtual void MoveKinematic(const Vector3f& targetPosition, const glm::quat& targetRotation, float deltaSeconds) override;

		virtual void SetTranslation(const Vector3f& translation) override;
		virtual Vector3f GetTranslation() const override;

		virtual void SetRotation(const glm::quat& rotation) override;
		virtual glm::quat GetRotation() const override;

		virtual void Rotate(const Vector3f& inRotationTimesDeltaTime) override;

	private:
		virtual void OnAxisLockUpdated(bool forceWake) override;

	private:
		void CreateStaticBody(JPH::BodyInterface& bodyInterface);
		void CreateDynamicBody(JPH::BodyInterface& bodyInterface);

		void CreateAxisLockConstraint(JPH::Body& body);

		void Release();

	private:
		JPH::BodyID m_BodyID;
		JPH::EMotionType m_OldMotionType = JPH::EMotionType::Static;

		glm::vec3 m_KinematicTargetPosition = { 0.0f, 0.0f, 0.0f };
		glm::quat m_KinematicTargetRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		float m_KinematicTargetTime = 0.0f;

		JPH::SixDOFConstraint* m_AxisLockConstraint = nullptr;

	private:
		friend class JoltScene;
	};

}
