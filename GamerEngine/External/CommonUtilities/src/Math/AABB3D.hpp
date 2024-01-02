#pragma once
#include "Frustum.h"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include <cassert>

namespace CommonUtilities
{
	template <typename T>
	class AABB3D
	{
	public:
		constexpr AABB3D() = default;
		constexpr AABB3D(const AABB3D<T>& aAABB3D);
		constexpr AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);
		constexpr AABB3D(const Vector3<T>& aCenter, const T& aExtentsX, const T& aExtentsY, const T& aExtentsZ);

		constexpr AABB3D<T> operator+(const Vector3<T>& aVec);

		constexpr void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		constexpr void InitWithCenterAndExtents(const Vector3<T>& aCenter, const Vector3<T>& aExtents);
		constexpr bool IsInside(const Vector3<T>& aPosition) const;

		constexpr const Vector3<T>& GetMin() const;
		constexpr const Vector3<T>& GetMax() const;
		constexpr Vector3<T> GetCenter() const;
		constexpr Vector3<T> GetExtents() const;

		constexpr AABB3D<T> Transform(const Matrix4x4<T>& aTransform) const;

		constexpr bool Intersects(const AABB3D<T>& aAABB) const;
		constexpr bool IsOnFrustum(const TFrustum<T>& aFrustum) const;
		constexpr bool IsOnFrustum(const TFrustum<T>& aFrustum, const Matrix4x4<T>& aTransform) const;
		constexpr bool IsOnOrInFrontOfPlane(const Plane<T>& aPlane) const;
	private:
		Vector3<T> myMin{ 0.0f, 0.0f, 0.0f };
		Vector3<T> myMax { 0.0f, 0.0f, 0.0f };
	};

	using AABB = AABB3D<float>;

	template<typename T>
	constexpr inline AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		InitWithMinAndMax(aAABB3D.myMin, aAABB3D.myMax);
	}

	template<typename T>
	constexpr inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		InitWithMinAndMax(aMin, aMax);
	}

	template <typename T>
	constexpr inline AABB3D<T>::AABB3D(const Vector3<T>& aCenter, const T& aExtentsX, const T& aExtentsY, const T& aExtentsZ)
	{
		InitWithCenterAndExtents(aCenter, Vector3<T>{ aExtentsX, aExtentsY, aExtentsZ});
	}

	template <typename T>
	constexpr inline AABB3D<T> AABB3D<T>::operator+(const Vector3<T>& aVec)
	{
		AABB3D<T> result(myMin + aVec, myMax + aVec);
		return result;
	}

	template<typename T>
	constexpr inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		assert(aMin.x <= aMax.x && aMin.y <= aMax.y && aMin.z <= aMax.z && "Min is greater than max!");
		myMin = aMin;
		myMax = aMax;
	}

	template <typename T>
	constexpr void AABB3D<T>::InitWithCenterAndExtents(const Vector3<T>& aCenter, const Vector3<T>& aExtents)
	{
		myMin = aCenter - aExtents;
		myMax = aCenter + aExtents;
	}

	template<typename T>
	constexpr inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if(aPosition.x < myMin.x)
		{
			return false;
		}

		if(aPosition.x > myMax.x)
		{
			return false;
		}

		if(aPosition.y < myMin.y)
		{
			return false;
		}

		if(aPosition.y > myMax.y)
		{
			return false;
		}

		if(aPosition.z < myMin.z)
		{
			return false;
		}

		if(aPosition.z > myMax.z)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	constexpr inline const Vector3<T>& AABB3D<T>::GetMin() const
	{
		return myMin;
	}
	template<typename T>
	constexpr inline const Vector3<T>& AABB3D<T>::GetMax() const
	{
		return myMax;
	}

	template <typename T>
	constexpr Vector3<T> AABB3D<T>::GetCenter() const
	{
		return (myMax + myMin) * static_cast<T>(0.5);
	}

	template <typename T>
	constexpr Vector3<T> AABB3D<T>::GetExtents() const
	{
		return myMax - GetCenter();
	}

	template <typename T>
	constexpr AABB3D<T> AABB3D<T>::Transform(const Matrix4x4<T>& aTransform) const
	{
		Vector3<T> center = (myMax + myMin) * 0.5f;
		Vector3<T> extents = myMax - center;

		Vector3<T> worldCenter = aTransform.TransformPoint(center);

		Vector3<T> transformRight = aTransform[0] * extents.x;
		Vector3<T> transformUp = aTransform[1] * extents.y;
		Vector3<T> transformForward = aTransform[2] * extents.z;

		T transformedExtentX = static_cast<T>(std::abs(Vector3<T>::Dot(Vector3<T>::Right(), transformRight)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Right(), transformUp)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Right(), transformForward)));

		T transformedExtentY = static_cast<T>(std::abs(Vector3<T>::Dot(Vector3<T>::Up(), transformRight)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Up(), transformUp)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Up(), transformForward)));

		T transformedExtentZ = static_cast<T>(std::abs(Vector3<T>::Dot(Vector3<T>::Forward(), transformRight)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Forward(), transformUp)) +
			std::abs(Vector3<T>::Dot(Vector3<T>::Forward(), transformForward)));

		return { worldCenter, transformedExtentX * aTransform.GetScale().x, transformedExtentY * aTransform.GetScale().y, transformedExtentZ * aTransform.GetScale().z };
	}


	template <typename T>
	constexpr bool AABB3D<T>::Intersects(const AABB3D<T>& aAABB) const
	{
		return (myMin.x <= aAABB.myMax.x &&
			myMax.x >= aAABB.myMin.x &&
			myMin.y <= aAABB.myMax.y &&
			myMin.z <= aAABB.myMax.z &&
			myMax.z >= aAABB.myMin.z);
	}


	template <typename T>
	constexpr bool AABB3D<T>::IsOnFrustum(const TFrustum<T>& aFrustum) const
	{
		return IsOnOrInFrontOfPlane(aFrustum.FarPlane) &&
			IsOnOrInFrontOfPlane(aFrustum.NearPlane) &&
			IsOnOrInFrontOfPlane(aFrustum.RightPlane) &&
			IsOnOrInFrontOfPlane(aFrustum.LeftPlane) &&
			IsOnOrInFrontOfPlane(aFrustum.TopPlane) &&
			IsOnOrInFrontOfPlane(aFrustum.BottomPlane);
	}

	template <typename T>
	constexpr bool AABB3D<T>::IsOnFrustum(const TFrustum<T>& aFrustum, const Matrix4x4<T>& aTransform) const
	{
		return Transform(aTransform).IsOnFrustum(aFrustum);
	}

	template <typename T>
	constexpr bool AABB3D<T>::IsOnOrInFrontOfPlane(const Plane<T>& aPlane) const
	{
		Vector3<T> planeNormal = aPlane.GetNormal();
		Vector3<T> center = GetCenter();
		Vector3<T> extents = myMax - center;

		T radius = extents.x * std::abs(planeNormal.x) + extents.y * std::abs(planeNormal.y) + extents.z * std::abs(planeNormal.z);
		return -radius <= aPlane.GetSignedDistanceToPlane(center);
	}
}
