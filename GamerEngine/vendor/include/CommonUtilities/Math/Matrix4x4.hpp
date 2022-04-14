#pragma once
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <array>

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

		std::array<std::array<T, 4>, 4> myMatrix;
	};

	template <class T>
	Matrix4x4<T>::Matrix4x4()
	{
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				if (i == j)
				{
					myMatrix[i - 1][j - 1] = 1;
				}
				else
				{
					myMatrix[i - 1][j - 1] = 0;
				}
			}
		}
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				myMatrix[i - 1][j - 1] = aMatrix(i, j);
			}
		}
	}

	template <class T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <class T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template<typename T>
	bool operator==(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix != aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator==(const Matrix4x4<T>& aMatrixZero, const Matrix4x4<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix != aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator!=(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix == aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool operator!=(const Matrix4x4<T>& aMatrixZero, const Matrix4x4<T>& aMatrixOne)
	{
		if (aMatrixZero.myMatrix == aMatrixOne.myMatrix)
		{
			return false;
		}

		return true;
	}

	template <typename T>
	Matrix4x4<T> operator+(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		Matrix4x4<T> result;
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{

				result(i, j) = aMatrixZero(i, j) + aMatrixOne(i, j);

			}
		}

		return result;
	}

	template <typename T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrixZero, const Matrix4x4<T>& aMatrixOne)
	{
		Matrix4x4<T> result;
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				result(i, j) = aMatrixZero(i, j) - aMatrixOne(i, j);
			}
		}

		return result;
	}

	template <typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrixZero, const Matrix4x4<T>& aMatrixOne)
	{
		Matrix4x4<T> multiply;
		T sum = 0;
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{

				for (unsigned int k = 1; k <= 4; ++k)
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
	Vector3<T> operator*(const Matrix4x4<T>& aMatrix, Vector3<T>& aVector)
	{
		Vector3<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return result;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix4x4<T> aMatrix)
	{
		Vector3<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return result;
	}

	template <typename T>
	Vector4<T> operator*(const Matrix4x4<T>& aMatrix, Vector4<T>& aVector)
	{
		Vector4<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		result.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);

		return result;
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T> aMatrix)
	{
		Vector4<T> result;

		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		result.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);

		return result;
	}

	template <typename T>
	void operator+=(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				aMatrixZero(i, j) += aMatrixOne(i, j);
			}
		}
	}

	template <typename T>
	void operator-=(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				aMatrixZero(i, j) -= aMatrixOne(i, j);
			}
		}
	}

	template <typename T>
	void operator*=(Matrix4x4<T>& aMatrixZero, Matrix4x4<T>& aMatrixOne)
	{
		Matrix4x4<T> multiply;
		T sum = 0;
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				for (unsigned int k = 1; k <= 4; ++k)
				{
					sum += aMatrixZero(i, k) * aMatrixOne(k, j);
				}

				multiply(i, j) = sum;
				sum = 0;
			}
		}

		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				aMatrixZero(i, j) = multiply(i, j);
			}
		}
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;

		matrix(2, 2) = cos(aAngleInRadians);
		matrix(2, 3) = sin(aAngleInRadians);
		matrix(3, 2) = -sin(aAngleInRadians);
		matrix(3, 3) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;

		matrix(1, 1) = cos(aAngleInRadians);
		matrix(1, 3) = -sin(aAngleInRadians);
		matrix(3, 1) = sin(aAngleInRadians);
		matrix(3, 3) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> matrix;

		matrix(1, 1) = cos(aAngleInRadians);
		matrix(1, 2) = sin(aAngleInRadians);
		matrix(2, 1) = -sin(aAngleInRadians);
		matrix(2, 2) = cos(aAngleInRadians);

		return matrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> transpose;
		for (unsigned int i = 1; i <= 4; ++i)
		{
			for (unsigned int j = 1; j <= 4; ++j)
			{
				transpose(i, j) = aMatrixToTranspose(j, i);
			}
		}
		return transpose;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> out;

		if (aTransform(1, 1) != 1.0f || aTransform(3, 3) != 1.0f)
		{
			for (unsigned int i = 1; i <= 4; ++i)
			{
				for (unsigned int j = 1; j <= 4 - 1; ++j)
				{
					if (aTransform(i, j) != 0.0f)
					{
						out(j, i) = aTransform(i, j);
					}
					else
					{
						out(j, i) = -aTransform(i, j);
					}
				}
			}
		}


		if (aTransform(4, 1) != 0.0f)
		{
			out(4, 1) = -aTransform(4, 1);
			out(4, 2) = -aTransform(4, 2);
			out(4, 3) = -aTransform(4, 3);
			
		}



		return out;
	}
}
