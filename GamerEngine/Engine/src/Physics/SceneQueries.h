// #pragma once
//
// #include "PhysicsShapes.h"
// #include "Utilites/Pointers.h"
//
// namespace Hazel {
//
// 	struct SceneQueryHit
// 	{
// 		uint64_t HitEntity = 0;
// 		Vector3f Position = Vector3f(0.0f);
// 		Vector3f Normal = Vector3f(0.0f);
// 		float Distance = 0.0f;
// 		Ref<PhysicsShape> HitCollider = nullptr;
//
// 		void Clear()
// 		{
// 			HitEntity = 0;
// 			Position = Vector3f(std::numeric_limits<float>::max());
// 			Normal = Vector3f(std::numeric_limits<float>::max());
// 			Distance = std::numeric_limits<float>::max();
// 			HitCollider = nullptr;
// 		}
// 	};
//
// 	using ExcludedEntityMap = std::unordered_set<UUID>;
//
// 	struct RayCastInfo
// 	{
// 		Vector3f Origin;
// 		Vector3f Direction;
// 		float MaxDistance;
// 		ExcludedEntityMap ExcludedEntities;
// 	};
//
// 	enum class ShapeCastType { Box, Sphere, Capsule };
//
// 	struct ShapeCastInfo
// 	{
// 		ShapeCastInfo(ShapeCastType castType)
// 			: m_Type(castType) {}
//
// 		Vector3f Origin = Vector3f(0.0f);
// 		Vector3f Direction = Vector3f(0.0f);
// 		float MaxDistance = 0.0f;
// 		ExcludedEntityMap ExcludedEntities;
//
// 		ShapeCastType GetCastType() const { return m_Type; }
//
// 	private:
// 		ShapeCastType m_Type;
// 	};
//
// 	struct BoxCastInfo : public ShapeCastInfo
// 	{
// 		BoxCastInfo() : ShapeCastInfo(ShapeCastType::Box) {}
//
// 		Vector3f HalfExtent = Vector3f(0.0f);
// 	};
//
// 	struct SphereCastInfo : public ShapeCastInfo
// 	{
// 		SphereCastInfo() : ShapeCastInfo(ShapeCastType::Sphere) {}
//
// 		float Radius = 0.0f;
// 	};
//
// 	struct CapsuleCastInfo : public ShapeCastInfo
// 	{
// 		CapsuleCastInfo() : ShapeCastInfo(ShapeCastType::Capsule) {}
//
// 		float HalfHeight = 0.0f;
// 		float Radius = 0.0f;
// 	};
//
// 	struct ShapeOverlapInfo
// 	{
// 		ShapeOverlapInfo(ShapeCastType castType)
// 			: m_Type(castType)
// 		{
// 		}
//
// 		Vector3f Origin = Vector3f(0.0f);
//
// 		ExcludedEntityMap ExcludedEntities;
//
// 		ShapeCastType GetCastType() const { return m_Type; }
//
// 	private:
// 		ShapeCastType m_Type;
// 	};
//
// 	struct BoxOverlapInfo : public ShapeOverlapInfo
// 	{
// 		BoxOverlapInfo() : ShapeOverlapInfo(ShapeCastType::Box) {}
//
// 		Vector3f HalfExtent = Vector3f(0.0f);
// 	};
//
// 	struct SphereOverlapInfo : public ShapeOverlapInfo
// 	{
// 		SphereOverlapInfo() : ShapeOverlapInfo(ShapeCastType::Sphere) {}
//
// 		float Radius = 0.0f;
// 	};
//
// 	struct CapsuleOverlapInfo : public ShapeOverlapInfo
// 	{
// 		CapsuleOverlapInfo() : ShapeOverlapInfo(ShapeCastType::Capsule) {}
//
// 		float HalfHeight = 0.0f;
// 		float Radius = 0.0f;
// 	};
//
// }
