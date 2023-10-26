#pragma once
#include "Matrix4x4.hpp"
#include "Plane.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	const float DegToRad = static_cast<float>(3.141f / 180.0f);


	template <typename T>
	struct TFrustum
	{
		Plane<T> TopPlane;
		Plane<T> BottomPlane;

		Plane<T> RightPlane;
		Plane<T> LeftPlane;

		Plane<T> FarPlane;
		Plane<T> NearPlane;

		Vector3<float> FarTopLeft;
		Vector3<float> FarTopRight;
		Vector3<float> FarBottomLeft;
		Vector3<float> FarBottomRight;

		Vector3<float> NearTopLeft;
		Vector3<float> NearTopRight;
		Vector3<float> NearBottomLeft;
		Vector3<float> NearBottomRight;
	};

	using Frustum = TFrustum<float>;

	template <typename T>
	inline TFrustum<T> CreateFrustumFromCamera(Matrix4x4<T> aCameraTransform, T aVFovDeg, T aHFovDeg, T aNearPlane, T aFarPlane)
	{
		TFrustum<T> frustum;

		T farHeightHalf = std::tanf(static_cast<T>(aVFovDeg) * 0.5f * static_cast<T>(DegToRad)) * static_cast<T>(aFarPlane);
		T farWidthHalf = std::tanf(static_cast<T>(aHFovDeg) * 0.5f * static_cast<T>(DegToRad)) * static_cast<T>(aFarPlane);

		T nearHeightHalf = std::tanf(static_cast<T>(aVFovDeg) * 0.5f * static_cast<T>(DegToRad)) * static_cast<T>(aNearPlane);
		T nearWidthHalf = std::tanf(static_cast<T>(aHFovDeg) * 0.5f * static_cast<T>(DegToRad)) * static_cast<T>(aNearPlane);

		Vector3<T> cameraTranslation = aCameraTransform[3];
		Vector3<T> cameraRight = Vector3<T>::Normalize(aCameraTransform[0]);
		Vector3<T> cameraUp = Vector3<T>::Normalize(aCameraTransform[1]);
		Vector3<T> cameraForward = Vector3<T>::Normalize(aCameraTransform[2]);

		Vector3<T> farCenter = cameraTranslation + cameraForward * aFarPlane;
		Vector3<T> nearCenter = cameraTranslation + cameraForward * aNearPlane;

		frustum.FarTopLeft = farCenter + cameraUp * farHeightHalf - cameraRight * farWidthHalf;
		frustum.FarTopRight = farCenter + cameraUp * farHeightHalf + cameraRight * farWidthHalf;
		frustum.FarBottomLeft = farCenter - cameraUp * farHeightHalf - cameraRight * farWidthHalf;
		frustum.FarBottomRight = farCenter - cameraUp * farHeightHalf + cameraRight * farWidthHalf;

		frustum.NearTopLeft = nearCenter + cameraUp * nearHeightHalf - cameraRight * nearWidthHalf;
		frustum.NearTopRight = nearCenter + cameraUp * nearHeightHalf + cameraRight * nearWidthHalf;
		frustum.NearBottomLeft = nearCenter - cameraUp * nearHeightHalf - cameraRight * nearWidthHalf;
		frustum.NearBottomRight = nearCenter - cameraUp * nearHeightHalf + cameraRight * nearWidthHalf;

		frustum.FarPlane = { farCenter, -cameraForward };
		frustum.NearPlane = { nearCenter, cameraForward };

		Vector3<T> topPlaneNormal = Vector3<T>::Cross((frustum.FarTopLeft - frustum.FarTopRight), (frustum.NearTopLeft - frustum.FarTopLeft)).Normalized();
		frustum.TopPlane = { frustum.FarTopLeft, topPlaneNormal };

		Vector3<T> bottomPlaneNormal = Vector3<T>::Cross((frustum.FarBottomRight - frustum.FarBottomLeft), (frustum.NearBottomRight - frustum.FarBottomRight)).Normalized();
		frustum.BottomPlane = { frustum.FarBottomRight, bottomPlaneNormal };

		Vector3<T> rightPlaneNormal = Vector3<T>::Cross((frustum.FarTopRight - frustum.FarBottomRight), (frustum.NearTopRight - frustum.FarTopRight)).Normalized();
		frustum.RightPlane = { frustum.FarTopRight, rightPlaneNormal };

		Vector3<T> leftPlaneNormal = Vector3<T>::Cross((frustum.FarBottomLeft - frustum.FarTopLeft), (frustum.NearBottomLeft - frustum.FarBottomLeft)).Normalized();
		frustum.LeftPlane = { frustum.FarBottomLeft, leftPlaneNormal };

		return frustum;
	}
}
