#pragma once
#include "Vector.h"
#include <array>
#include <cassert>
#include <math.h>
#include <cmath>

namespace CommonUtilities
{

	template<class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		static Matrix4x4<T> Slerp(Matrix4x4<T> aMatrixOne, Matrix4x4<T> aMatrixTwo, float aTime);
		static Matrix4x4<T> BuildTransform(Vector3<T> aTranslate, Vector3<T> aRotation, Vector3<T> aScale);
		void DecomposeTransform(Vector3<T>& aTranslate, Vector3<T>& aRotation, Vector3<T>& aScale);

		Vector3<T> GetXAxis()
		{
			return Vector3<T>(myMatrix[0][0], myMatrix[0][1], myMatrix[0][2]);
		}

		Vector3<T> GetYAxis()
		{
			return Vector3<T>(myMatrix[1][0], myMatrix[1][1], myMatrix[1][2]);
		}

		Vector3<T> GetZAxis()
		{
			return Vector3<T>(myMatrix[2][0], myMatrix[2][1], myMatrix[2][2]);
		}

		Vector3<T> GetPosition() const;
		Vector4<T> GetRotation() const;
		Vector4<T> GetScale() const;



		std::array<float, 16> GetFloatPtr();
		void SetFloatPtr(float* aFloatPtr);
		void SetPosition(Vector4<T> aPosition);


		std::array<std::array<T, 4>, 4> myMatrix;
	};



	template<typename T>
	std::array<float, 16> Matrix4x4<T>::GetFloatPtr()
	{
		return std::array<float, 16>{
			myMatrix[0][0],	myMatrix[0][1],	myMatrix[0][2],	myMatrix[0][3],
			myMatrix[1][0],	myMatrix[1][1],	myMatrix[1][2],	myMatrix[1][3],
			myMatrix[2][0],	myMatrix[2][1],	myMatrix[2][2],	myMatrix[2][3],
			myMatrix[3][0],	myMatrix[3][1],	myMatrix[3][2],	myMatrix[3][3]
		};
	}

	template <class T>
	void Matrix4x4<T>::SetFloatPtr(float* aFloatPtr)
	{
		for(int row = 0; row < 4; row++)
		{
			for(int column = 0; column < 4; column++)
			{
				myMatrix[row][column] = aFloatPtr[row * column];
			}
		}
	}

	template <typename T>
	Matrix4x4<T>::Matrix4x4()
	{
		for(int row = 1; row <= 4; ++row)
		{

			for(int column = 1; column <= 4; ++column)
			{
				if(row == column)
				{
					myMatrix[row - 1][column - 1] = static_cast<T>(1.0f);
				}
				else
				{
					myMatrix[row - 1][column - 1] = static_cast<T>(0.0f);
				}
			}
		}
	}

	template<typename T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix4)
	{
		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				myMatrix[row - 1][column - 1] = aMatrix4(row, column);
			}
		}
	}


	template <typename T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <typename T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow - 1][aColumn - 1];
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

	template <typename T>
	void operator*=(Matrix4x4<T>& aFirstMatrix4, const Matrix4x4<T>& aSecondMatrix4)
	{
		Matrix4x4<T> result;
		T sum = 0;

		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{

				for(int temp = 1; temp <= 4; ++temp)
				{
					sum += aFirstMatrix4(row, temp) * aSecondMatrix4(temp, column);
				}

				result(row, column) = sum;
				sum = 0;
			}
		}

		for(int row = 1; row <= 4; ++row)
		{
			for(int column = 1; column <= 4; ++column)
			{
				aFirstMatrix4(row, column) = result(row, column);
			}
		}
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myMatrix[1][1] = cos(aAngleInRadians);
		matrix.myMatrix[1][2] = sin(aAngleInRadians);
		matrix.myMatrix[2][1] = -sin(aAngleInRadians);
		matrix.myMatrix[2][2] = cos(aAngleInRadians);

		return matrix;
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myMatrix[0][0] = cos(aAngleInRadians);
		matrix.myMatrix[0][2] = -sin(aAngleInRadians);
		matrix.myMatrix[2][0] = sin(aAngleInRadians);
		matrix.myMatrix[2][2] = cos(aAngleInRadians);

		return matrix;
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myMatrix[0][0] = cos(aAngleInRadians);
		matrix.myMatrix[0][1] = sin(aAngleInRadians);
		matrix.myMatrix[1][0] = -sin(aAngleInRadians);
		matrix.myMatrix[1][1] = cos(aAngleInRadians);

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
		myMatrix[3][0] = aPosition.x;
		myMatrix[3][1] = aPosition.y;
		myMatrix[3][2] = aPosition.z;
		myMatrix[3][3] = aPosition.w;
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

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::BuildTransform(Vector3<T> aTranslate, Vector3<T> aRotation, Vector3<T> aScale)
	{
		Matrix4x4<T> output = Matrix4x4<T>();

		output(1, 1) *= aScale.x;
		output(2, 2) *= aScale.y;
		output(3, 3) *= aScale.z;

		aRotation *= 3.14159f / 180;
		output *= Matrix4x4<T>::CreateRotationAroundX(aRotation.x);
		output *= Matrix4x4<T>::CreateRotationAroundY(aRotation.y);
		output *= Matrix4x4<T>::CreateRotationAroundZ(aRotation.z);

		output(4, 1) = aTranslate.x;
		output(4, 2) = aTranslate.y;
		output(4, 3) = aTranslate.z;
		output(4, 4) = 1;

		return output;
	}

	template <class T>
	void Matrix4x4<T>::DecomposeTransform(Vector3<T>& aTranslate, Vector3<T>& aRotation, Vector3<T>& aScale)
	{
		aTranslate.x = myMatrix[3][0];
		aTranslate.y = myMatrix[3][1];
		aTranslate.z = myMatrix[3][2];

		float radToDeg = 180.0f / 3.14159f;
		aRotation.x = radToDeg * std::atan2f(myMatrix[1][2], myMatrix[2][2]);
		aRotation.y = radToDeg * std::atan2f(-myMatrix[0][2], std::sqrtf(myMatrix[1][2] * myMatrix[1][2] + myMatrix[2][2] * myMatrix[2][2]));
		aRotation.z = radToDeg * std::atan2f( myMatrix[0][1], myMatrix[0][0]);

		aScale.x = myMatrix[0][0];
		aScale.y = myMatrix[1][1];
		aScale.z = myMatrix[2][2];
	}

	template <class T>
	Vector3<T> Matrix4x4<T>::GetPosition() const
	{
		return Vector3<float>(myMatrix[3][0], myMatrix[3][1], myMatrix[3][2]);
	}
}
