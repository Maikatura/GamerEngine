#pragma once
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <array>

namespace CommonUtilities
{
	template<typename T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

		std::array<T, 9> myMatrix;

	};

	template <typename T>
	Matrix3x3<T>::Matrix3x3()
	{
		myMatrix = {
			static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
		};

	}

	template <typename T>
	Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	template <typename T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		int index = 0;
		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				myMatrix[index] = aMatrix(i + 1, j + 1);
				index++;
			}
		}

	}

	template <typename T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix[(aRow - 1) * 3 + (aColumn - 1)];
	}

	template <typename T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[(aRow - 1) * 3 + (aColumn - 1)];
	}

	template<typename T>
	bool operator==(Matrix3x3<T>& aMatrixZero, Matrix3x3<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix != aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator==(const Matrix3x3<T>& aMatrixZero, const Matrix3x3<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix != aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator!=(Matrix3x3<T>& aMatrixZero, Matrix3x3<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix == aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator!=(const Matrix3x3<T>& aMatrixZero, const Matrix3x3<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix == aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template <typename T>
	Matrix3x3<T> operator+(Matrix3x3<T>& aMatrixZero, Matrix3x3<T>& aMatrixOne)
	{
		Matrix3x3<T> result;
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{

				result(i, j) = aMatrixZero(i, j) + aMatrixOne(i, j);

			}
		}

		return result;
	}

	template <typename T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrixZero, const Matrix3x3<T>& aMatrixOne)
	{
		Matrix3x3<T> result;
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{
				result(i, j) = aMatrixZero(i, j) - aMatrixOne(i, j);
			}
		}

		return result;
	}

	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrixZero, const Matrix3x3<T>& aMatrixOne)
	{
		Matrix3x3<T> multiply;
		T sum = 0;
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{

				for (unsigned int k = 1; k <= 3; ++k)
				{
					sum += aMatrixZero(i, k) * aMatrixOne(k, j);
				}

				multiply(i, j) = sum;
				sum = 0;
			}
		}

		return multiply;
	}

	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, Vector3<T>& aVector)
	{
		Vector3<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return result;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T> aMatrix)
	{
		Vector3<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return result;
	}

	template <typename T>
	void operator+=(Matrix3x3<T>& aMatrixZero, Matrix3x3<T>& aMatrixOne)
	{
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{
				aMatrixZero(i, j) += aMatrixOne(i, j);
			}
		}
	}

	template <typename T>
	void operator-=( Matrix3x3<T>& aMatrixZero,  Matrix3x3<T>& aMatrixOne)
	{
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{
				aMatrixZero(i, j) -= aMatrixOne(i, j);
			}
		}
	}

	template <typename T>
	void operator*=(Matrix3x3<T>& aMatrixZero, Matrix3x3<T>& aMatrixOne)
	{
		Matrix3x3<T> multiply;
		T sum = 0;
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{

				for (unsigned int k = 1; k <= 3; ++k)
				{
					sum += aMatrixZero(i, k) * aMatrixOne(k, j);
				}

				multiply(i, j) = sum;
				sum = 0;
			}
		}

		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{
				aMatrixZero(i, j) = multiply(i, j);
			}
		}
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> matrix;

		matrix(2, 2) = cos(aAngleInRadians);
		matrix(2, 3) = sin(aAngleInRadians);
		matrix(3, 2) = -sin(aAngleInRadians);
		matrix(3, 3) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> matrix;

		matrix(1, 1) = cos(aAngleInRadians);
		matrix(1, 3) = -sin(aAngleInRadians);
		matrix(3, 1) = sin(aAngleInRadians);
		matrix(3, 3) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> matrix;

		matrix(1, 1) = cos(aAngleInRadians);
		matrix(1, 2) = sin(aAngleInRadians);
		matrix(2, 1) = -sin(aAngleInRadians);
		matrix(2, 2) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> transpose;
		for (unsigned int i = 1; i <= 3; ++i)
		{
			for (unsigned int j = 1; j <= 3; ++j)
			{
				transpose(i, j) = aMatrixToTranspose(j, i);
			}
		}
		return transpose;
	}
}


