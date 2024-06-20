#include "JoltUtils.h"

namespace GamerEngine::JoltUtils
{

	/*JPH::Vec3 ToJoltVector(const Vector3f& vector) { return JPH::Vec3(vector.x, vector.y, vector.z); }
	Vector3f ToJoltQuat(const glm::quat& quat) { return JPH::Quat(quat.x, quat.y, quat.z, quat.w); }

Vector3f FromJoltVector(const JPH::Vec3& vector) { return *(glm::vec3*)&vector; }
	Vector3f FromJoltQuat(const JPH::Quat& quat) { return *&quat; }

	JPH::EMotionQuality ToJoltMotionQuality(ECollisionDetectionType collisionType)
	{
		switch (collisionType)
		{
		case ECollisionDetectionType::Discrete: return JPH::EMotionQuality::Discrete;
		case ECollisionDetectionType::Continuous: return JPH::EMotionQuality::LinearCast;
		}

		return JPH::EMotionQuality::Discrete;
	}

	JPH::EMotionType ToJoltMotionType(EBodyType bodyType)
	{
		switch (bodyType)
		{
		case EBodyType::Static: return JPH::EMotionType::Static;
		case EBodyType::Dynamic: return JPH::EMotionType::Dynamic;
		case EBodyType::Kinematic: return JPH::EMotionType::Kinematic;
		}

		return JPH::EMotionType::Static;
	}*/

}
