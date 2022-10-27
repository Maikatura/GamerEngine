#pragma once
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Vector3
	{
	public:
		T x = 0;
		T y = 0;
		T z = 0;

		Vector3<T>(); //Creates a null-vector 
		Vector3<T>(const T& aX, const T& aY, const T& aZ); //Creates a vector (aX, aY, aZ) 
		Vector3<T>(const Vector3<T>& aVector) = default; //Copy constructor (compiler generated) 
		~Vector3<T>() = default; //Destructor (compiler generated)

		Vector3<T>& operator=(const Vector3<T>& aVector3) = default; 	//Assignment operator (compiler generated) 
		void operator=(const T& aScaler);

		T LengthSqr() const; //Returns the squared length of the vector 
		T Length() const; //Returns the length of the vector 

		Vector3<T> GetNormalized() const; //Returns a normalized copy of this 
		void Normalize(); //Normalizes the vector

		T Dot(const Vector3<T>& aVector) const; //Returns the dot product of this and aVector 
		Vector3<T> Cross(const Vector3<T>& aVector) const; //Returns the cross product of this and aVector

		Vector3<T> Blend(Vector3<T> aBlendState1, Vector3<T> aBlendState2, T aBlendAmount);
	};

	template <class T>
	Vector3<T>::Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	template <class T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template <class T>
	void Vector3<T>::operator=(const T& aScaler)
	{
		x = aScaler;
		y = aScaler;
		z = aScaler;
	}

	template <class T>
	T Vector3<T>::LengthSqr() const
	{
		return ((x * x) + (y * y) + (z * z));
	}

	template <class T>
	T Vector3<T>::Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	template <class T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length = Length();

		assert(length != 0 && "Length is 0");

		if(length != 0.0f)
		{
			T lengthInverse = 1 / length;
			return Vector3<T>(x * lengthInverse, y * lengthInverse, z * lengthInverse);
		}

		return Vector3<T>(x * 0.0f, y * 0.0f, z * 0.0f);
	}

	template <class T>
	void Vector3<T>::Normalize()
	{
		assert(x != 0 && "X is 0 and cant be normilized");
		assert(y != 0 && "X is 0 and cant be normilized");
		assert(z != 0 && "X is 0 and cant be normilized");

		Vector3<T> normalized = GetNormalized();

		x = normalized.x;
		y = normalized.y;
		z = normalized.z;

	}

	template <class T>
	T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return (x * aVector.x) + (y * aVector.y) + (z * aVector.z);
	}

	template <class T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return Vector3<T>(
			y * aVector.z - z * aVector.y,
			z * aVector.x - x * aVector.z,
			x * aVector.y - y * aVector.x);
	}

	template <class T>
	Vector3<T> Vector3<T>::Blend(Vector3<T> aBlendState1, Vector3<T> aBlendState2, T aBlendAmount)
	{
		const T channelXA = aBlendState1.x * aBlendAmount;
		const T channelXB = aBlendState2.x * (1 - aBlendAmount);
		T x = (channelXA + channelXB);

		const T channelYA = aBlendState1.y * aBlendAmount;
		const T channelYB = aBlendState2.y * (1 - aBlendAmount);
		T y = (channelYA + channelYB);

		const T channelZA = aBlendState1.z * aBlendAmount;
		const T channelZB = aBlendState2.z * (1 - aBlendAmount);
		T z = (channelZA + channelZB);

		return { x, y, z };
	}

	//Returns the vector sum of aVector0 and aVector1 
	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	//Returns the vector difference of aVector0 and aVector1 
	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	//Returns the vector aVector multiplied by the scalar aScalar 
	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar 
	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Scaler can't be 0 since you cant divide by 0");

		return Vector3<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1) 
	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	template <class T>
	bool operator==(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return (aVector0.x == aVector1.x) && (aVector0.y == aVector1.y) && (aVector0.z == aVector1.z);
	}

	template <class T>
	bool operator!=(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return (aVector0.x != aVector1.x) && (aVector0.y != aVector1.y) && (aVector0.z != aVector1.z);
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1) 
	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
	}

	//Equivalent to setting aVector to (aVector * aScalar) 
	template <class T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar) 
	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Scaler can't be 0 since you cant divide by 0");

		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
	}

}


