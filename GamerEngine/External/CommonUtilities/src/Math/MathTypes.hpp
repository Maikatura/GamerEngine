#pragma once
#include <Math/Vector.h>
#include <Math/Matrix.h>

typedef CommonUtilities::Vector2<float> Vector2f;
typedef CommonUtilities::Vector3<float> Vector3f;
typedef CommonUtilities::Vector4<float> Vector4f;
typedef CommonUtilities::Matrix3x3<float> Matrix3x3f;
typedef CommonUtilities::Matrix4x4<float> Matrix4x4f;
typedef CommonUtilities::Vector2<int> Vector2i;
typedef CommonUtilities::Vector3<int> Vector3i;
typedef CommonUtilities::Vector4<int> Vector4i;
typedef CommonUtilities::Matrix3x3<int> Matrix3x3i;
typedef CommonUtilities::Matrix4x4<int> Matrix4x4i;
typedef CommonUtilities::Vector2<unsigned int> Vector2ui;
typedef CommonUtilities::Vector3<unsigned int> Vector3ui;
typedef CommonUtilities::Vector4<unsigned int> Vector4ui;
typedef CommonUtilities::Matrix3x3<unsigned int> Matrix3x3ui;
typedef CommonUtilities::Matrix4x4<unsigned int> Matrix4x4ui;
typedef CommonUtilities::Vector2<double> Vector2d;
typedef CommonUtilities::Vector3<double> Vector3d;
typedef CommonUtilities::Vector4<double> Vector4d;
typedef CommonUtilities::Matrix3x3<double> Matrix3x3d;
typedef CommonUtilities::Matrix4x4<double> Matrix4x4d;

// Useful Tools To Convert

template<class T>
CommonUtilities::Vector3<T> ToVector3(const CommonUtilities::Vector4<T>& aVectorToConvert)
{
	return CommonUtilities::Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
}

template<class T>
CommonUtilities::Vector3<T> ToDegrees(const CommonUtilities::Vector3<T>& aVectorToConvert)
{
	float radToDegree = 180.0f / 3.14159f;
	CommonUtilities::Vector3<T> returnVector3 = CommonUtilities::Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
	returnVector3 *= radToDegree;

	return returnVector3;
}


template<class T>
CommonUtilities::Vector3<T> ToRadians(const CommonUtilities::Vector3<T>& aVectorToConvert)
{
	float degreeToRad = 3.14159f / 180.0f;
	CommonUtilities::Vector3<T> returnVector3 = CommonUtilities::Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
	returnVector3 *= degreeToRad;

	return returnVector3;
}

template<class T>
CommonUtilities::Matrix4x4<T> ComposeFromTRS(const CommonUtilities::Vector3<T>& aTranslation, const CommonUtilities::Quaternion<T>& aRotationQuat, const CommonUtilities::Vector3<T>& aScale)
{
	return CommonUtilities::Matrix4x4<T>::CreateScale(aScale)
		* aRotationQuat.GetRotationMatrix4x4()
		* CommonUtilities::Matrix4x4<T>::CreateTranslation(aTranslation);
}

