#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <math.h>
#include "Vector.h"
#include "Matrix3x3.hpp"




namespace CommonUtilities
{

	template<class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		Matrix4x4(T aValue);
		Matrix4x4(std::array<T, 16> myMatrix);

		T& operator()(const size_t aRow, const size_t aColumn);
		const T& operator()(const size_t aRow, const size_t aColumn) const;
		T& operator()(const size_t anIndex);
		const T& operator()(const size_t anIndex) const;


		T* Ptr();
		const T* Ptr() const;

		Vector3<T> operator[](const size_t anIndex);
		const Vector3<T> operator[](const size_t anIndex) const;
		Vector3<T> ToEularAngles();


		

		static Matrix4x4<T> CreateLookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up);
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		static Matrix4x4<T> Slerp(Matrix4x4<T> aMatrixOne, Matrix4x4<T> aMatrixTwo, float aTime);

		static Matrix4x4<T> CreateTranslation(const Vector3<T>& aTranslation);
		static Matrix4x4<T> CreateScale(const Vector3<T>& aScale);

		static Matrix4x4<T> CreateProjection(T aHorizontalFoV, T aNearPlane, T aFarPlane, Vector2<unsigned int> aResolution);
		static Matrix4x4<T> CreateOrthographicProjection(const T& aLeft, const T& aRight, const T& aBottom, const T& aTop, const T& aNear, const T& aFar);

		constexpr Vector3<T> TransformPoint(const Vector3<T>& aPoint) const;
		constexpr Vector3<T> TransformPoint(const Vector4<T>& aPoint) const; //Transform as w = 1

		constexpr Vector3<T> TransformVector(const Vector4<T>& aVector) const; // Transform as w = 0
		constexpr static Matrix4x4<T> CreatePerspectiveHFoVResolution(const T& aHorizontalFoV, const Vector2<uint32_t>& aResolution, const T& aNearClipPlane, const T& aFarClipPlane);
		constexpr static Matrix4x4<T> AffineInverse(const Matrix4x4<T>& aAffineMatrix);

		void DecomposeTransform(Vector3<T>& aTranslate, Vector3<T>& aRotation, Vector3<T>& aScale);
		Vector3<T> GetTranslation();

		bool ContainsPosition(Vector3<T>& aTranslate);




		static Matrix4x4<T> CreateRollPitchYawMatrix(Vector3<T>& aRotation);

		Vector3<T> GetRight();
		Vector3<T> GetUp() const;
		Vector3<T> GetForward();
		Vector4<T> GetForwardW();

		Vector3<T> GetPosition();
		Vector4<T> GetPositionW() const;
		Vector3<T> GetRotation();
		Vector3<T> GetScale() const;



		friend std::ostream& operator<<(std::ostream& os, const Matrix4x4<T>& obj)
		{
			os << obj.myMatrix[0] << " " << obj.myMatrix[1] << " " << obj.myMatrix[2] << " " << obj.myMatrix[3] << std::endl;
			os << obj.myMatrix[4] << " " << obj.myMatrix[5] << " " << obj.myMatrix[6] << " " << obj.myMatrix[7] << std::endl;
			os << obj.myMatrix[8] << " " << obj.myMatrix[9] << " " << obj.myMatrix[10] << " " << obj.myMatrix[11] << std::endl;
			os << obj.myMatrix[12] << " " << obj.myMatrix[13] << " " << obj.myMatrix[14] << " " << obj.myMatrix[15] << std::endl;
			return os;
		}


		void SetPosition(Vector4<T> aPosition);

		std::array<T, 16> myMatrix;
	};



	template <typename T>
	constexpr Vector3<T> Matrix4x4<T>::TransformVector(const Vector4<T>& aVector) const
	{
		Vector4<T> output = operator*(Vector4<T>(aVector.x, aVector.y, aVector.z, 0.0f), *this);
		return Vector3<T>{output.x, output.y, output.z};
	}

	template <typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreatePerspectiveHFoVResolution(const T& aHorizontalFoV, const Vector2<uint32_t>& aResolution, const T& aNearClipPlane, const T& aFarClipPlane)
	{
		assert(aNearClipPlane < aFarClipPlane && "Near clip plane can't be larger than far clip plane");
		assert(aNearClipPlane >= static_cast<T>(0.001) && "Near clip plane is too small!");

		T hFoVRad = aHorizontalFoV * 0.017453f;
		T vFoVRad = static_cast<T>(2.0) * std::atan(std::tan(hFoVRad / static_cast<T>(2.0))) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x));

		T xScale = static_cast<T>(1.0) / std::tan(hFoVRad * static_cast<T>(0.5));
		T yScale = static_cast<T>(1.0) / std::tan(vFoVRad * static_cast<T>(0.5));

		T Q = aFarClipPlane / (aFarClipPlane - aNearClipPlane);

		Matrix4x4<T> projection;
		projection(1, 1) = xScale;
		projection(2, 2) = yScale;
		projection(3, 3) = Q;
		projection(3, 4) = static_cast<T>(1.0) / Q;
		projection(4, 3) = Q * -aNearClipPlane;
		projection(4, 4) = static_cast<T>(0.0);

		return projection;
	}

	template <typename T>
	constexpr Vector3<T> Matrix4x4<T>::TransformPoint(const Vector3<T>& aPoint) const
	{
		Vector4<T> returnValue = operator*(Vector4<T>(aPoint.x, aPoint.y, aPoint.z, 1.0f), *this);
		return Vector3<T>(aPoint.x, aPoint.y, aPoint.z);
	}

	template <typename T>
	constexpr Vector3<T> Matrix4x4<T>::TransformPoint(const Vector4<T>& aPoint) const
	{
		Vector4<T> returnValue = operator*(Vector4<T>(aPoint.x, aPoint.y, aPoint.z, 1.0f), *this);
		return Vector3<T>{ returnValue.x, returnValue.y, returnValue.z };
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateOrthographicProjection(const T& aLeft, const T& aRight, const T& aBottom,
		const T& aTop, const T& aNear, const T& aFar)
	{
		Matrix4x4<T> projection;

		projection(1, 1) = static_cast<T>(2.0) / (aRight - aLeft);
		projection(2, 2) = static_cast<T>(2.0) / (aTop - aBottom);
		projection(3, 3) = static_cast<T>(1.0) / (aFar - aNear);
		projection(4, 1) = static_cast<T>(-((aRight + aLeft) / (aRight - aLeft)));
		projection(4, 2) = static_cast<T>(-((aTop + aBottom) / (aTop - aBottom)));
		projection(4, 3) = static_cast<T>(-(aNear / (aNear - aFar)));
		projection(4, 4) = static_cast<T>(1);

		return projection;
	}


	template <class T>
	Vector3<T> Matrix4x4<T>::GetScale() const
	{
		return { myMatrix[0], myMatrix[5], myMatrix[10] };
	}

	template <typename T>
	Matrix4x4<T>::Matrix4x4()
	{
		myMatrix = {
			static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
		};
		
	}

	template<typename T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		for(size_t i = 0; i < 16; i++)
		{
			myMatrix[i] = aMatrix(i);
		}
	}

	template<typename T>
	Matrix4x4<T>::Matrix4x4(T aValue)
	{
		for (int i = 0; i < 16; i++)
		{
			myMatrix[i] = aValue;
		}
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(std::array<T, 16> myMatrix)
	{
		this->myMatrix = myMatrix;
	}


	template <typename T>
	T& Matrix4x4<T>::operator()(const size_t aRow, const size_t aColumn)
	{
		return myMatrix[(aRow - 1) * 4 + (aColumn - 1)];
	}

	template <typename T>
	const T& Matrix4x4<T>::operator()(const size_t aRow, const size_t aColumn) const
	{
		return myMatrix[(aRow - 1) * 4 + (aColumn - 1)];
	}

	template <class T>
	const T& Matrix4x4<T>::operator()(const size_t anIndex) const
	{
		return myMatrix[anIndex];
	}

	template <class T>
	T* Matrix4x4<T>::Ptr()
	{
		return &myMatrix[0];
	}

	template <class T>
	const T* Matrix4x4<T>::Ptr() const
	{
		return &myMatrix[0];
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::operator[](const size_t anIndex)
	{
		return Vector3<T>(myMatrix[(4 * anIndex) + 0], myMatrix[(4 * anIndex) + 1], myMatrix[(4 * anIndex) + 2]);
	}

	template <class T>
	const Vector3<T> Matrix4x4<T>::operator[](const size_t anIndex) const
	{
		return Vector3<T>(myMatrix[(4 * anIndex) + 0], myMatrix[(4 * anIndex) + 1], myMatrix[(4 * anIndex) + 2]);
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::ToEularAngles()
	{
		Vector3<T> angles;

		// Calculate yaw (around Y-axis)
		angles.y = std::atan2(-myMatrix[2], std::sqrt(myMatrix[0] * myMatrix[0] + myMatrix[1] * myMatrix[1]));

		// Calculate pitch (around X-axis)
		if (std::abs(myMatrix[2]) < 1.0) 
		{
			angles.x = std::atan2(myMatrix[6], myMatrix[10]);
			angles.z = std::atan2(myMatrix[1], myMatrix[0]);
		}
		else 
		{
			angles.x = std::atan2(myMatrix[6], myMatrix[5]);
			angles.z = 0.0f;
		}

		// Convert radians to degrees if needed
		angles.x = angles.x * 180.0f / 3.14159f;
		angles.y = angles.y * 180.0f / 3.14159f;
		angles.z = angles.z * 180.0f / 3.14159f;

		return angles;
	}

	
	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateLookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
	{
		Vector3 zAxis = eye.Normalized() - target.Normalized();
		Vector3 xAxis = up.Cross(zAxis).Normalized();
		Vector3 yAxis = zAxis.Cross(xAxis);

		Matrix4x4 viewMatrix;

		viewMatrix(0) = xAxis.x;
		viewMatrix(1) = xAxis.y;
		viewMatrix(2) = xAxis.z;
		viewMatrix(3) = -xAxis.Dot(eye);
				  
		viewMatrix(4) = yAxis.x;
		viewMatrix(5) = yAxis.y;
		viewMatrix(6) = yAxis.z;
		viewMatrix(7) = -yAxis.Dot(eye);
				  
		viewMatrix(8) = zAxis.x;
		viewMatrix(9) = zAxis.y;
		viewMatrix(10) = zAxis.z;
		viewMatrix(11) = -zAxis.Dot(eye);
				  
		viewMatrix(12) = 0.0f;
		viewMatrix(13) = 0.0f;
		viewMatrix(14) = 0.0f;
		viewMatrix(15) = 1.0f;

		return viewMatrix;
	}

	template <class T>
	T& Matrix4x4<T>::operator()(const size_t anIndex)
	{
		return myMatrix[anIndex];
	}

	template<typename T>
	bool operator==(Matrix4x4<T>& aFirstMatrix4, Matrix4x4<T>& aSecondMatrix4)
	{
		if(aFirstMatrix4.myMatrix != aSecondMatrix4.myMatrix)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool operator==(const Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		if(aFirstMatrix4.myMatrix != aSecondMatrix4.myMatrix)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool operator!=(Matrix4x4<T>& aFirstMatrix4, Matrix4x4<T>& aSecondMatrix4)
	{
		if(aFirstMatrix4.myMatrix == aSecondMatrix4.myMatrix)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool operator!=(const Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		if(aFirstMatrix4.myMatrix == aSecondMatrix4.myMatrix)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result(aFirstMatrix4);
		result += aSecondMatrix4;

		return result;
	}


	template<typename T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result(aFirstMatrix4);

		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				result(row, column) -= aSecondMatrix4(row, column);
			}
		}
		return result;
	}

	template<typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result;

		for(int row = 1; row <= 4; row++)
		{
			for(int column = 1; column <= 4; column++)
			{
				if(row == column) result(row, column) = 0;
				for(int i = 1; i <= 4; i++)
				{
					result(row, column) += aFirstMatrix4(row, i) * aSecondMatrix4(i, column);
				}
			}
		}
		return result;
	}

	template<typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aFirstMatrix4, const Matrix3x3<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result;

		for (int row = 1; row <= 3; row++)
		{
			for (int column = 1; column <= 3; column++)
			{
				if (row == column) result(row, column) = 0;
				for (int i = 1; i <= 3; i++)
				{
					result(row, column) += aFirstMatrix4(row, i) * aSecondMatrix4(i, column);
				}
			}
		}
		return result;
	}

	template <typename T>
	Vector3<T> operator*(const Matrix4x4<T>& aMatrix4, const Vector3<T>& aVector3)
	{
		Vector3<T> result;
		result.x = aVector3.x * aMatrix4(1, 1) + aVector3.y * aMatrix4(2, 1) + aVector3.z * aMatrix4(3, 1);
		result.y = aVector3.x * aMatrix4(1, 2) + aVector3.y * aMatrix4(2, 2) + aVector3.z * aMatrix4(3, 2);
		result.z = aVector3.x * aMatrix4(1, 3) + aVector3.y * aMatrix4(2, 3) + aVector3.z * aMatrix4(3, 3);

		return result;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector3, const Matrix4x4<T> aMatrix4)
	{
		Vector3<T> result;
		result.x = aVector3.x * aMatrix4(1, 1) + aVector3.y * aMatrix4(2, 1) + aVector3.z * aMatrix4(3, 1);
		result.y = aVector3.x * aMatrix4(1, 2) + aVector3.y * aMatrix4(2, 2) + aVector3.z * aMatrix4(3, 2);
		result.z = aVector3.x * aMatrix4(1, 3) + aVector3.y * aMatrix4(2, 3) + aVector3.z * aMatrix4(3, 3);

		return result;
	}

	template <typename T>
	Vector4<T> operator*(const Matrix4x4<T>& aMatrix4, const Vector4<T>& aVector3)
	{
		Vector4<T> result;
		result.x = aVector3.x * aMatrix4(1, 1) + aVector3.y * aMatrix4(2, 1) + aVector3.z * aMatrix4(3, 1) + aVector3.w * aMatrix4(4, 1);
		result.y = aVector3.x * aMatrix4(1, 2) + aVector3.y * aMatrix4(2, 2) + aVector3.z * aMatrix4(3, 2) + aVector3.w * aMatrix4(4, 2);
		result.z = aVector3.x * aMatrix4(1, 3) + aVector3.y * aMatrix4(2, 3) + aVector3.z * aMatrix4(3, 3) + aVector3.w * aMatrix4(4, 3);
		result.w = aVector3.x * aMatrix4(1, 4) + aVector3.y * aMatrix4(2, 4) + aVector3.z * aMatrix4(3, 4) + aVector3.w * aMatrix4(4, 4);

		return result;
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector3, const Matrix4x4<T> aMatrix4)
	{
		Vector4<T> result;
		result.x = aVector3.x * aMatrix4(1, 1) + aVector3.y * aMatrix4(2, 1) + aVector3.z * aMatrix4(3, 1) + aVector3.w * aMatrix4(4, 1);
		result.y = aVector3.x * aMatrix4(1, 2) + aVector3.y * aMatrix4(2, 2) + aVector3.z * aMatrix4(3, 2) + aVector3.w * aMatrix4(4, 2);
		result.z = aVector3.x * aMatrix4(1, 3) + aVector3.y * aMatrix4(2, 3) + aVector3.z * aMatrix4(3, 3) + aVector3.w * aMatrix4(4, 3);
		result.w = aVector3.x * aMatrix4(1, 4) + aVector3.y * aMatrix4(2, 4) + aVector3.z * aMatrix4(3, 4) + aVector3.w * aMatrix4(4, 4);

		return result;
	}

	

	template<typename T>
	void operator+=(Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				aFirstMatrix4(row, column) += aSecondMatrix4(row, column);
			}
		}
	}

	template<typename T>
	void operator-=(Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				aFirstMatrix4(row, column) -= aSecondMatrix4(row, column);
			}
		}
	}

	template<typename T>
	void operator*=(Vector4<T>& aLval, const Matrix4x4<T>& aRval)
	{
		Vector4<T> output;
		output.x = aLval.x * aRval(1, 1);
		output.y = aLval.x * aRval(1, 2);
		output.z = aLval.x * aRval(1,3);
		output.w = aLval.x * aRval(1,4);
		output.x += aLval.y * aRval(2,1);
		output.y += aLval.y * aRval(2,2);
		output.z += aLval.y * aRval(2,3);
		output.w += aLval.y * aRval(2,4);
		output.x += aLval.z * aRval(3,1);
		output.y += aLval.z * aRval(3,2);
		output.z += aLval.z * aRval(3,3);
		output.w += aLval.z * aRval(3,4);
		output.x += aLval.w * aRval(4,1);
		output.y += aLval.w * aRval(4,2);
		output.z += aLval.w * aRval(4,3);
		output.w += aLval.w * aRval(4,4);
		aLval = output; // Switched for row major
	}

	template <typename T>
	void operator*=(Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result;
		
		for (int row = 1; row <= 4; ++row)
		{
			for (int column = 1; column <= 4; ++column)
			{
				T sum = 0;

				for (int temp = 1; temp <= 4; ++temp)
				{
					sum += aFirstMatrix4(row, temp) * aSecondMatrix4(temp, column);
				}

				result(row, column) = sum;
			}
		}

		aFirstMatrix4 = result;

	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix(2, 2) = cos(aAngleInRadians);
		matrix(3, 2) = -sin(aAngleInRadians);
		matrix(2, 3) = sin(aAngleInRadians);
		matrix(3, 3) = cos(aAngleInRadians);

		return matrix;
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix(3, 3) = cos(aAngleInRadians);
		matrix(1, 3) = -sin(aAngleInRadians);
		matrix(3, 1) = sin(aAngleInRadians);
		matrix(1, 1) = cos(aAngleInRadians);

		return matrix;
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix(1, 1) = cos(aAngleInRadians);
		matrix(2, 1) = -sin(aAngleInRadians);
		matrix(1, 2) = sin(aAngleInRadians);
		matrix(2, 2) = cos(aAngleInRadians);
		return matrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> transpose;
		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				transpose(row, column) = aMatrixToTranspose(column, row);
			}
		}

		return transpose;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4 temp(aTransform);
		Vector4<T> pos(aTransform(4, 1), aTransform(4, 2), aTransform(4, 3), aTransform(4, 4));
		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;

		temp = Transpose(temp);

		pos = Vector4<T>(-pos.x, -pos.y, -pos.z, pos.w) * temp;
		temp(4, 1) = pos.x;
		temp(4, 2) = pos.y;
		temp(4, 3) = pos.z;

		return temp;
	}

	template <class T>
	void Matrix4x4<T>::SetPosition(Vector4<T> aPosition)
	{
		myMatrix[12] = aPosition.x;
		myMatrix[13] = aPosition.y;
		myMatrix[14] = aPosition.z;
		myMatrix[15] = aPosition.w;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Slerp(Matrix4x4<T> aMatrixOne, Matrix4x4<T> aMatrixTwo, float aTime)
	{
		Matrix4x4<T> result;
		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				result(row, column) = std::lerp(aMatrixOne(row, column), aMatrixTwo(row, column), aTime);
			}
		}
		return result;
	}

	

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T>& aTranslation)
	{
		Matrix4x4<T> translationMatrix;
		translationMatrix(4, 1) = aTranslation.x;
		translationMatrix(4, 2) = aTranslation.y;
		translationMatrix(4, 3) = aTranslation.z;
		return translationMatrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScale(const Vector3<T>& aScale)
	{
		Matrix4x4<T> scaleMatrix;

		scaleMatrix(1, 1) = aScale.x;
		scaleMatrix(2, 2) = aScale.y;
		scaleMatrix(3, 3) = aScale.z;

		return scaleMatrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateProjection(T aHorizontalFoV, T aNearPlane, T aFarPlane, Vector2<unsigned int> aResolution)
	{
		assert(aNearPlane < aFarPlane);
		assert(aNearPlane >= 0.01f);


		Matrix4x4<T> projection;


		const float hFoVRad = aHorizontalFoV * (0.01745f);
		const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

		const float myXScale = 1 / std::tanf(hFoVRad * 0.5f);
		const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
		const float Q = aFarPlane / (aFarPlane - aNearPlane);

		projection(1, 1) = myXScale;
		projection(2, 2) = myYScale;
		projection(3, 3) = Q;
		projection(3, 4) = 1.0f / Q;
		projection(4, 3) = -Q * aNearPlane;
		projection(4, 4) = 0.0f;

		return projection;
	}

	template <class T>
	void Matrix4x4<T>::DecomposeTransform(Vector3<T>& aTranslate, Vector3<T>& aRotation, Vector3<T>& aScale)
	{
		Matrix4x4<T> mat = *this;

		aTranslate.x = mat(4,1);
		aTranslate.y = mat(4,2);
		aTranslate.z = mat(4,3);

		float radToDeg = 180.0f / 3.14159f;
		aRotation.x = radToDeg * std::atan2f(mat(2,3), mat(3,3));
		aRotation.y = radToDeg * std::atan2f(-mat[0][2], std::sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));
		aRotation.z = radToDeg * std::atan2f(mat[0][1], mat[0][0]);

		aScale.x = mat(1,1);
		aScale.y = mat(2,2);
		aScale.z = mat(3,3);
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::GetTranslation()
	{
		return Vector3<T>{ myMatrix[12], myMatrix[13], myMatrix[14] };
	}

	template <class T>
	bool Matrix4x4<T>::ContainsPosition(Vector3<T>& aTranslate)
	{
		return true;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRollPitchYawMatrix(Vector3<T>& aRotation)
	{
		Matrix4x4<T> output = Matrix4x4<T>();

		aRotation *= 3.14159f / 180;
		output *= Matrix4x4<T>::CreateRotationAroundX(aRotation.x);
		output *= Matrix4x4<T>::CreateRotationAroundY(aRotation.y);
		output *= Matrix4x4<T>::CreateRotationAroundZ(aRotation.z);

		return output;
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::GetRight()
	{
		return Vector3<T>{ myMatrix[0], myMatrix[1], myMatrix[2] };
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::GetUp() const
	{
		return Vector3<T>{ myMatrix[4], myMatrix[5], myMatrix[6] };;
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::GetForward()
	{
		return Vector3<T>{ myMatrix[8], myMatrix[9], myMatrix[10] };
	}

	template <class T>
	Vector4<T> Matrix4x4<T>::GetForwardW()
	{
		return Vector4<T>{ myMatrix[8], myMatrix[9], myMatrix[10], myMatrix[11] };
	}


	template <class T>
	Vector3<T> Matrix4x4<T>::GetPosition()
	{
		return Vector3<T>{ myMatrix[12], myMatrix[13], myMatrix[14] };
	}

	template <class T>
	Vector4<T> Matrix4x4<T>::GetPositionW() const
	{
		return Vector4<T>{ myMatrix[12], myMatrix[13], myMatrix[14], myMatrix[15] };
	}

	/*template <class T>
	Vector3<T> Matrix4x4<T>::GetRotation() const
	{
		Vector3<T> aRotation = { 0,0,0 };

		T radToDeg = 180.0f / 3.14159f;
		aRotation.x = radToDeg * static_cast<T>(std::atan2(myMatrix[6], myMatrix[10]));
		aRotation.y = radToDeg * static_cast<T>(std::atan2(-myMatrix[2], myMatrix[0]));
		aRotation.z = radToDeg * static_cast<T>(std::atan2(myMatrix[1], myMatrix[5]));
		return aRotation;
	}*/

	template <class T>
	Vector3<T> Matrix4x4<T>::GetRotation()
	{
		Vector3<T> aRotation = {0,0,0};

		aRotation.x = static_cast<T>(std::atan2(myMatrix[6], myMatrix[10]));
		aRotation.y = static_cast<T>(std::atan2(-myMatrix[2], myMatrix[0]));
		aRotation.z = static_cast<T>(std::atan2(myMatrix[1], myMatrix[5]));
		return aRotation;
	}

	template <typename T>
	constexpr inline Matrix4x4<T> Matrix4x4<T>::AffineInverse(const Matrix4x4<T>& aAffineMatrix)
	{
		T determinantA = aAffineMatrix(1, 1) * ((aAffineMatrix(2, 2) * aAffineMatrix(3, 3)) - (aAffineMatrix(2, 3) * aAffineMatrix(3, 2)));
		T determinantB = aAffineMatrix(1, 2) * ((aAffineMatrix(2, 1) * aAffineMatrix(3, 3)) - (aAffineMatrix(2, 3) * aAffineMatrix(3, 1)));
		T determinantC = aAffineMatrix(1, 3) * ((aAffineMatrix(2, 1) * aAffineMatrix(3, 2)) - (aAffineMatrix(2, 2) * aAffineMatrix(3, 1)));
		T determinant = determinantA - determinantB + determinantC;

		assert(determinant != static_cast<T>(0) && "Matrix does not have an inverse");

		T det11 = (aAffineMatrix(2, 2) * aAffineMatrix(3, 3) - aAffineMatrix(2, 3) * aAffineMatrix(3, 2));
		T det12 = -(aAffineMatrix(1, 2) * aAffineMatrix(3, 3) - aAffineMatrix(1, 3) * aAffineMatrix(3, 2));
		T det13 = (aAffineMatrix(1, 2) * aAffineMatrix(2, 3) - aAffineMatrix(1, 3) * aAffineMatrix(2, 2));

		T det21 = -(aAffineMatrix(2, 1) * aAffineMatrix(3, 3) - aAffineMatrix(2, 3) * aAffineMatrix(3, 1));
		T det22 = (aAffineMatrix(1, 1) * aAffineMatrix(3, 3) - aAffineMatrix(1, 3) * aAffineMatrix(3, 1));
		T det23 = -(aAffineMatrix(1, 1) * aAffineMatrix(2, 3) - aAffineMatrix(1, 2) * aAffineMatrix(2, 1));

		T det31 = (aAffineMatrix(2, 1) * aAffineMatrix(3, 2) - aAffineMatrix(2, 2) * aAffineMatrix(3, 1));
		T det32 = -(aAffineMatrix(1, 1) * aAffineMatrix(3, 2) - aAffineMatrix(1, 2) * aAffineMatrix(3, 1));
		T det33 = (aAffineMatrix(1, 1) * aAffineMatrix(2, 2) - aAffineMatrix(1, 2) * aAffineMatrix(2, 1));

		//inverse rotation and scale matrix
		Matrix4x4<T> inversedMatrix;

		T oneOverDeterminant = static_cast<T>(1.0) / determinant;
		inversedMatrix(1, 1) = det11 * oneOverDeterminant;
		inversedMatrix(1, 2) = det21 * oneOverDeterminant;
		inversedMatrix(1, 3) = det31 * oneOverDeterminant;

		inversedMatrix(2, 1) = det12 * oneOverDeterminant;
		inversedMatrix(2, 2) = det22 * oneOverDeterminant;
		inversedMatrix(2, 3) = det32 * oneOverDeterminant;

		inversedMatrix(3, 1) = det13 * oneOverDeterminant;
		inversedMatrix(3, 2) = det23 * oneOverDeterminant;
		inversedMatrix(3, 3) = det33 * oneOverDeterminant;



		 Vector4<T> point = Vector4<T>{ inversedMatrix.TransformPoint(-aAffineMatrix[3]), static_cast<T>(1.0) };
		 inversedMatrix(12) = point.x;
		 inversedMatrix(13) = point.y;
		 inversedMatrix(14) = point.z;
		 inversedMatrix(15) = point.w;


		return inversedMatrix;
	}

}
