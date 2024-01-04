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


namespace CommonUtilities
{
	template<class T>
	Vector3<T> ToVector3(const Vector4<T>& aVectorToConvert)
	{
		return Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
	}

	template<class T>
	Vector3<T> ToDegrees(const Vector3<T>& aVectorToConvert)
	{
		Vector3<T> returnVector3 = Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
		returnVector3 *= RadToDeg;

		return returnVector3;
	}


	template<class T>
	Vector3<T> ToRadians(const Vector3<T>& aVectorToConvert)
	{
		float degreeToRad = DegToRad;
		Vector3<T> returnVector3 = Vector3<T>(aVectorToConvert.x, aVectorToConvert.y, aVectorToConvert.z);
		returnVector3 *= degreeToRad;

		return returnVector3;
	}

	template<class T>
	Matrix4x4<T> ComposeFromTRS(const Vector3<T>& aTranslation, const Quaternion<T>& aRotationQuat, const Vector3<T>& aScale)
	{
		return Matrix4x4<T>::CreateScale(aScale) * aRotationQuat.GetRotationMatrix4x4() * Matrix4x4<T>::CreateTranslation(aTranslation);
	}

	template<class T>
	Matrix4x4<T> CreateCubeMapViewMatrix(const Vector3<T>& aCameraPos, const Vector3<T>& aTargetPos, const Vector3<T>& aUp, const Vector3<T>& aRight)
	{
		Matrix4x4<T> returnMatrix;

		Vector3<T> forward = Vector3<T>::Normalize(aTargetPos - aCameraPos);

		Vector3<T> orthoUp = Vector3<T>::Normalize(aUp - Vector3<T>::Dot(aUp, forward) * forward);

		Vector3<T> orthoRight = Vector3<T>::Cross(orthoUp, forward);

		return Matrix4x4<T>::CreateLookAt(aCameraPos, aTargetPos, orthoUp);
	}
}



