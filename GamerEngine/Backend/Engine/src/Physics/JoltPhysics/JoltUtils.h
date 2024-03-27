#pragma once

#include "Hazel/Physics/PhysicsTypes.h"

#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Body/MotionQuality.h>
#include <Jolt/Physics/Body/MotionType.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Math/MathTypes.hpp"

namespace GamerEngine::JoltUtils
{

	JPH::Vec3 ToJoltVector(const Vector3f& vector);
	JPH::Quat ToJoltQuat(const glm::quat& quat);

	Vector3f FromJoltVector(const JPH::Vec3& vector);
	glm::quat FromJoltQuat(const JPH::Quat& quat);

	template<typename T, typename U>
	JPH::Ref<T> CastJoltRef(JPH::RefConst<U> ref)
	{
		return JPH::Ref<T>(static_cast<T*>(const_cast<U*>(ref.GetPtr())));
	}

	template<typename T, typename U>
	JPH::Ref<T> CastJoltRef(const JPH::Ref<U> ref)
	{
		return JPH::Ref<T>(static_cast<T*>(const_cast<U*>(ref.GetPtr())));
	}

	JPH::EMotionQuality ToJoltMotionQuality(ECollisionDetectionType collisionType);
	JPH::EMotionType ToJoltMotionType(EBodyType bodyType);

}
