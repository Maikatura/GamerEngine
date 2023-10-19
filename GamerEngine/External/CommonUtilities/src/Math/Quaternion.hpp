#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <cmath>
#include <ostream>

#define PI_NUMBER 3.14f
#define DEG_TO_RAD 0.0174532925f

namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4;

	template <typename T>
	class Matrix3x3;

	template <typename T>
	class Quaternion
	{
	public:
		T w{};
		T x{};
		T y{};
		T z{};

		constexpr Quaternion<T>();
		constexpr Quaternion<T>(const T& aW, const T& aX, const T& aY, const T& aZ);
		constexpr explicit Quaternion<T>(const Vector3<T>& aEulers);
		constexpr explicit Quaternion<T>(const Matrix4x4<T>& aRotationMatrix);
		constexpr explicit Quaternion<T>(const Matrix3x3<T>& aRotationMatrix);
		constexpr explicit Quaternion<T>(const Vector3<T>& aAxis, const T& aAngle);

		constexpr Quaternion<T>(const Quaternion<T>& aOther) = default;
		constexpr Quaternion<T>& operator=(const Quaternion<T>& aOther) = default;

		constexpr T Length() const;
		constexpr T LengthSqr() const;

		constexpr Quaternion<T> Normalized();
		constexpr void Normalize();

		constexpr Vector3<T> Eulers() const;

		constexpr Vector3<T> Forward() const;
		constexpr Vector3<T> Right() const;
		constexpr Vector3<T> Up() const;

		constexpr Matrix3x3<T> GetRotationMatrix3x3() const;
		constexpr Matrix4x4<T> GetRotationMatrix4x4() const;

		constexpr Quaternion<T>& operator-();

		constexpr Vector3<T> Axis() const;

		T* ValuePointer() { return &w; }
		const T* ValuePointer() const { return &w; }

		static constexpr Quaternion<T> Identity();
		constexpr static Quaternion<T> Conjugate(const Quaternion<T>& aQuaternion);
		constexpr static Quaternion<T> Inverse(const Quaternion<T>& aQuaternion);
		constexpr static Quaternion<T> LookRotation(const Vector3<T>& aForward, const Vector3<T>& aUp = Vector3<T>(0.0f, 1.0f, 0.0f));
		constexpr static Quaternion<T> FromRotationMatrix3x3(const Matrix3x3<T>& aMatrix);
		constexpr static Quaternion<T> FromRotationMatrix4x4(const Matrix4x4<T>& aMatrix);
		constexpr static Quaternion<T> FromEulers(const Vector3<T>& aEulers); //Eulers in radians
		constexpr static Quaternion<T> FromAxisAngle(const Vector3<T>& aAxis, const T& aAngle); //Axis needs to be normalized.
		constexpr static Quaternion<T> Lerp(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB, const T& aT);
		constexpr static Quaternion<T> Slerp(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB, const T& aT);
		constexpr static T Dot(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB);
		constexpr static Quaternion<T> Normalize(const Quaternion<T>& aQuaternion);
		constexpr static Quaternion<T> DeltaRotationLocal(const Quaternion<T>& aFromRotation, const Quaternion<T>& aToRotation);
		constexpr static Quaternion<T> DeltaRotationWorld(const Quaternion<T>& aFromRotation, const Quaternion<T>& aToRotation);
		constexpr static Quaternion<T> CalculateAverageQuaternion(const Quaternion<T>* aQuaternions, size_t aCount);
	};

	using Quat = CommonUtilities::Quaternion<float>;
	using Quatf = CommonUtilities::Quaternion<float>;
	using Quatd = CommonUtilities::Quaternion<double>;

	using Quaternionf = CommonUtilities::Quaternion<float>;
	using Quaterniond = CommonUtilities::Quaternion<double>;

	template <typename T>
	std::ostream& operator<<(std::ostream& aOut, const Quaternion<T>& aQuaternion)
	{
		aOut << "(W: " << aQuaternion.w << " | X: " << aQuaternion.x << " | Y: " << aQuaternion.y << " | Z: " << aQuaternion.z << ")";
		return aOut;
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion()
		: w(static_cast<T>(1)),
		x(static_cast<T>(0)),
		y(static_cast<T>(0)),
		z(static_cast<T>(0))
	{}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ)
		: w(aW),
		x(aX),
		y(aY),
		z(aZ)
	{}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aEulers)
	{
		Quaternion<T> quat = Quaternion<T>::FromEulers(aEulers);
		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Matrix4x4<T>& aRotationMatrix)
	{
		Quaternion<T> quat = Quaternion<T>::FromRotationMatrix4x4(aRotationMatrix);
		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Matrix3x3<T>& aRotationMatrix)
	{
		Quaternion<T> quat = Quaternion<T>::FromRotationMatrix3x3(aRotationMatrix);
		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;
	}

	template <typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aAxis, const T& aAngle)
	{
		Quaternion<T> quat = Quaternion<T>::FromAxisAngle(aAxis, aAngle);
		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;
	}

	template <typename T>
	constexpr T Quaternion<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}

	template <typename T>
	constexpr T Quaternion<T>::LengthSqr() const
	{
		return Dot(*this, *this);
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Normalized()
	{
		return Normalize(*this);
	}

	template <typename T>
	constexpr void Quaternion<T>::Normalize()
	{
		*this = Normalize(*this);
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Identity()
	{
		return Quaternion<T> {static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Conjugate(const Quaternion<T>& aQuaternion)
	{
		return Quaternion<T> { aQuaternion.w, -aQuaternion.x, -aQuaternion.y, -aQuaternion.z};
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Inverse(const Quaternion<T>& aQuaternion)
	{
		return Conjugate(Quat::Normalize(aQuaternion));
	}

	template <typename T>
	constexpr Vector3<T> Quaternion<T>::Forward() const
	{
		Vector3<T> forward;

		forward.x = static_cast<T>(2) * (x * z + w * y);
		forward.y = static_cast<T>(2) * (y * z - w * x);
		forward.z = static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y);

		return forward.Normalized();
	}

	template <typename T>
	constexpr Vector3<T> Quaternion<T>::Right() const
	{
		Vector3<T> right;

		right.x = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);
		right.y = static_cast<T>(2) * (x * y + w * z);
		right.z = static_cast<T>(2) * (x * z - w * y);

		return right.Normalized();
	}

	template <typename T>
	constexpr Vector3<T> Quaternion<T>::Up() const
	{
		Vector3<T> up;

		up.x = static_cast<T>(2) * (x * y - w * z);
		up.y = static_cast<T>(1) - static_cast<T>(2) * (x * x + z * z);
		up.z = static_cast<T>(2) * (y * z + w * x);

		return up.Normalized();
	}

	template <typename T>
	constexpr Matrix3x3<T> Quaternion<T>::GetRotationMatrix3x3() const
	{
		return Matrix3x3{ GetRotationMatrix4x4() };
	}

	template <typename T>
	constexpr Matrix4x4<T> Quaternion<T>::GetRotationMatrix4x4() const
	{
		Matrix4x4<T> rotationMatrix;

		rotationMatrix(1, 1) = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);
		rotationMatrix(1, 2) = static_cast<T>(2) * (x * y + w * z);
		rotationMatrix(1, 3) = static_cast<T>(2) * (x * z - w * y);

		rotationMatrix(2, 1) = static_cast<T>(2) * (x * y - w * z);
		rotationMatrix(2, 2) = static_cast<T>(1) - static_cast<T>(2) * (x * x + z * z);
		rotationMatrix(2, 3) = static_cast<T>(2) * (y * z + w * x);

		rotationMatrix(3, 1) = static_cast<T>(2) * (x * z + w * y);
		rotationMatrix(3, 2) = static_cast<T>(2) * (y * z - w * x);
		rotationMatrix(3, 3) = static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y);

		return rotationMatrix;
	}

	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator-()
	{
		w *= static_cast<T>(-1);
		x *= static_cast<T>(-1);
		y *= static_cast<T>(-1);
		z *= static_cast<T>(-1);

		return *this;
	}

	template <typename T>
	constexpr Vector3<T> Quaternion<T>::Axis() const
	{
		Vector3<T> result;
		Quaternion quat{ w, x, y, z };
		quat.Normalize();

		result.x = quat.x;
		result.y = quat.y;
		result.z = quat.z;

		return result;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::LookRotation(const Vector3<T>& aForward, const Vector3<T>& aUp)
	{
		Vector3<T> forward = aForward;
		Vector3<T> right = aUp.Cross(aForward);
		Vector3<T> up = forward.Cross(right);
		Matrix3x3<T> matrix;
		matrix[0] = right;
		matrix[1] = up;
		matrix[2] = forward;
		return Quaternion<T>::FromRotationMatrix3x3(matrix);
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::FromRotationMatrix3x3(const Matrix3x3<T>& aMatrix)
	{
		T m00 = aMatrix(1, 1);
		T m01 = aMatrix(2, 1);
		T m02 = aMatrix(3, 1);

		T m10 = aMatrix(1, 2);
		T m11 = aMatrix(2, 2);
		T m12 = aMatrix(3, 2);

		T m20 = aMatrix(1, 3);
		T m21 = aMatrix(2, 3);
		T m22 = aMatrix(3, 3);

		Quaternion<T> quatFromMatrix;

		T t = static_cast<T>(0);
		if(m22 < static_cast<T>(0))
		{
			if(m00 > m11)
			{
				t = static_cast<T>(1) + m00 - m11 - m22;

				quatFromMatrix.x = t;
				quatFromMatrix.y = m01 + m10;
				quatFromMatrix.z = m20 + m02;
				quatFromMatrix.w = m12 - m21;
			}
			else
			{
				t = static_cast<T>(1) - m00 + m11 - m22;
				quatFromMatrix.x = m01 + m10;
				quatFromMatrix.y = t;
				quatFromMatrix.z = m12 + m21;
				quatFromMatrix.w = m20 - m02;
			}
		}
		else
		{
			if(m00 < -m11)
			{
				t = static_cast<T>(1) - m00 - m11 + m22;
				quatFromMatrix.x = m20 + m02;
				quatFromMatrix.y = m12 + m21;
				quatFromMatrix.z = t;
				quatFromMatrix.w = m01 - m10;
			}
			else
			{
				t = static_cast<T>(1) + m00 + m11 + m22;
				quatFromMatrix.x = m12 - m21;
				quatFromMatrix.y = m20 - m02;
				quatFromMatrix.z = m01 - m10;
				quatFromMatrix.w = t;
			}
		}

		quatFromMatrix *= static_cast<T>(0.5) / std::sqrt(t);
		quatFromMatrix.w *= static_cast<T>(-1);
		quatFromMatrix.Normalize();

		return Quat(quatFromMatrix.w, quatFromMatrix.x, quatFromMatrix.y, quatFromMatrix.z);
	}

	//From: https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::FromRotationMatrix4x4(const Matrix4x4<T>& aMatrix)
	{
		return Quaternion<T>::FromRotationMatrix3x3(Matrix3x3<T>{ aMatrix });
	}

	//Degrees
	template <typename T>
	constexpr Vector3<T> Quaternion<T>::Eulers() const
	{
		Vector3<T> angles{ 0.0f , 0.0f, 0.0f };

		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		angles.x = std::atan2(sinr_cosp, cosr_cosp);

		// Pitch (y-axis rotation)
		float sinp = 2 * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			angles.y = std::copysign(PI_NUMBER / 2, sinp); // Use 90 degrees if out of range
		else
			angles.y = std::asin(sinp);

		// Yaw (z-axis rotation)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		angles.z = std::atan2(siny_cosp, cosy_cosp);

		return angles;
	}

	template <typename T>
	inline T NormalizeAngle(T angleInDegrees) {
		// Ensure the angle is in the 0 to 360-degree range
		if (angleInDegrees < static_cast<T>(0.0f)) {
			angleInDegrees += static_cast<T>(360.0f);
		}
		else if (angleInDegrees >= static_cast<T>(360.0f)) {
			angleInDegrees -= static_cast<T>(360.0f);
		}
		return angleInDegrees;
	}

	// Explanation: https://math.stackexchange.com/a/2975462
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::FromEulers(const Vector3<T>& aEulers)
	{

		Vector3<T> halfAngle = aEulers * static_cast<T>(0.5);

		Quaternion<T> pitch{ std::cos(halfAngle.x), std::sin(halfAngle.x), static_cast<T>(0), static_cast<T>(0) };
		Quaternion<T> yaw{ std::cos(halfAngle.y), static_cast<T>(0), std::sin(halfAngle.y), static_cast<T>(0) };
		Quaternion<T> roll{ std::cos(halfAngle.z), static_cast<T>(0), static_cast<T>(0), std::sin(halfAngle.z) };

		Quaternion<T> result = roll * yaw * pitch;
		result.Normalize();

		return result;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::FromAxisAngle(const Vector3<T>& aAxis, const T& aAngle)
	{
		T halfAngle = aAngle * static_cast<T>(0.5);
		T sinHalfAngle = std::sin(halfAngle);

		Quaternion<T> result;
		result.w = std::cos(halfAngle);
		result.x = aAxis.x * sinHalfAngle;
		result.y = aAxis.y * sinHalfAngle;
		result.z = aAxis.z * sinHalfAngle;

		return result;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB, const T& aT)
	{
		return Quat();
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB, const T& aT)
	{
		/*Quaternion<T> quatB = aQuaternionB;

		T dotAB = Dot(aQuaternionA, quatB);

		if(dotAB < static_cast<T>(0))
		{
			quatB = -quatB;
			dotAB *= static_cast<T>(-1);
		}

		T dotABSqr = CommonUtilities::Min(dotAB * dotAB, static_cast<T>(1));

		T sinTheta = std::sqrt(static_cast<T>(1) - dotABSqr);

		if(sinTheta < static_cast<T>(0.0001))
		{
			return aQuaternionA;
		}*/

		//return ((sinTheta * (static_cast<T>(1) - aT)) / sinTheta) * aQuaternionA + ((sinTheta * aT) / sinTheta) * quatB;
		return Quaternion<T>();
	}

	template <typename T>
	constexpr T Quaternion<T>::Dot(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		return aQuaternionA.w * aQuaternionB.w + aQuaternionA.x * aQuaternionB.x + aQuaternionA.y * aQuaternionB.y + aQuaternionA.z * aQuaternionB.z;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::Normalize(const Quaternion<T>& aQuaternion)
	{
		Quaternion<T> normalized{ aQuaternion };

		T magnitude = aQuaternion.Length();

		if(magnitude != static_cast<T>(0))
		{
			normalized /= magnitude;
		}

		return normalized;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::DeltaRotationLocal(const Quaternion<T>& aFromRotation, const Quaternion<T>& aToRotation)
	{
		return Quat::Inverse(aFromRotation) * aToRotation;
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::DeltaRotationWorld(const Quaternion<T>& aFromRotation, const Quaternion<T>& aToRotation)
	{
		return aToRotation * Quat::Inverse(aFromRotation);
	}

	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::CalculateAverageQuaternion(const Quaternion<T>* aQuaternions, size_t aCount)
	{
		Quaternion<T> cumulativeRotation{ static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0) };
		Quaternion<T> averageRotation{ static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0) };

		for(size_t index = 0; index < aCount; index++)
		{
			Quaternion<T> nextRotation = aQuaternions[index];
			if(Quat::Dot(cumulativeRotation, nextRotation) < 0.0f)
			{
				nextRotation = -nextRotation;
			}

			cumulativeRotation += nextRotation;

			T addDeterminant = static_cast<T>(1.0) / (index + static_cast<T>(1));
			averageRotation = cumulativeRotation * addDeterminant;
			averageRotation.Normalize();
		}

		return averageRotation;
	}

	template <typename T, typename TScalar>
	constexpr inline Quaternion<T>& operator*=(Quaternion<T>& aQuaternionA, const TScalar& aScalar)
	{
		aQuaternionA.w *= aScalar;
		aQuaternionA.x *= aScalar;
		aQuaternionA.y *= aScalar;
		aQuaternionA.z *= aScalar;

		return aQuaternionA;
	}

	template <class T, typename TScalar>
	constexpr inline Quaternion<T> operator*(const Quaternion<T>& aQuaternionA, const TScalar& aScalar)
	{
		Quaternion<T> result = aQuaternionA;
		result *= aScalar;
		return result;
	}

	template <class T>
	constexpr inline Quaternion<T> operator*(const T& Scalar, const Quaternion<T>& aQuaternionA)
	{
		Quaternion<T> result = aQuaternionA;
		result *= Scalar;
		return result;
	}

	template <typename T, typename TScalar>
	constexpr inline Quaternion<T>& operator/=(Quaternion<T>& aQuaternionA, const TScalar& aScalar)
	{
		aQuaternionA.w /= aScalar;
		aQuaternionA.x /= aScalar;
		aQuaternionA.y /= aScalar;
		aQuaternionA.z /= aScalar;

		return aQuaternionA;
	}

	template <typename T, typename TScalar>
	constexpr inline Quaternion<T> operator/(const Quaternion<T>& aQuaternionA, const TScalar& Scalar)
	{
		Quaternion<T> result = aQuaternionA;
		result /= Scalar;
		return result;
	}

	template <typename T, typename TScalar>
	constexpr inline Quaternion<T> operator/(const TScalar& Scalar, const Quaternion<T>& aQuaternionA)
	{
		Quaternion<T> result = aQuaternionA;
		result /= Scalar;
		return result;
	}

	template <typename T>
	constexpr inline Quaternion<T>& operator*=(Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		T w = aQuaternionA.w;
		T x = aQuaternionA.x;
		T y = aQuaternionA.y;
		T z = aQuaternionA.z;

		aQuaternionA.w = (w * aQuaternionB.w) - (x * aQuaternionB.x) - (y * aQuaternionB.y) - (z * aQuaternionB.z);
		aQuaternionA.x = (w * aQuaternionB.x) + (x * aQuaternionB.w) + (y * aQuaternionB.z) - (z * aQuaternionB.y);
		aQuaternionA.y = (w * aQuaternionB.y) - (x * aQuaternionB.z) + (y * aQuaternionB.w) + (z * aQuaternionB.x);
		aQuaternionA.z = (w * aQuaternionB.z) + (x * aQuaternionB.y) - (y * aQuaternionB.x) + (z * aQuaternionB.w);

		return aQuaternionA;
	}

	template <class T>
	constexpr inline Quaternion<T> operator*(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		Quaternion<T> result = aQuaternionA;
		result *= aQuaternionB;
		return result;
	}

	template <class T>
	constexpr inline Vector3<T> operator*(const Quaternion<T>& aQuaternion, const Vector3<T>& aVector3)
	{
		Quat vectorAsQuat{ 0.0f, aVector3.x, aVector3.y, aVector3.z };
		Quaternion<T> result = aQuaternion * vectorAsQuat * Quat::Conjugate(aQuaternion);
		return Vector3<T> { result.x, result.y, result.z };
	}

	template <class T>
	constexpr inline Vector4<T> operator*(const Quaternion<T>& aQuaternion, const Vector4<T>& aVector4)
	{
		Quat vectorAsQuat{ aVector4.w, aVector4.x, aVector4.y, aVector4.z };
		Quaternion<T> result = aQuaternion * vectorAsQuat * Quat::Conjugate(aQuaternion);
		return Vector4<T> { result.x, result.y, result.z, result.w };
	}

	template <typename T>
	constexpr inline Quaternion<T>& operator+=(Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		aQuaternionA.w += aQuaternionB.w;
		aQuaternionA.x += aQuaternionB.x;
		aQuaternionA.y += aQuaternionB.y;
		aQuaternionA.z += aQuaternionB.z;

		return aQuaternionA;
	}

	template <class T>
	constexpr inline Quaternion<T> operator+(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		Quaternion<T> result = aQuaternionA;
		result += aQuaternionB;
		return result;
	}
}
