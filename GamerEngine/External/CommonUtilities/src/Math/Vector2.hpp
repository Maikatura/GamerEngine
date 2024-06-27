#pragma once
#include <cassert>
#include <iostream>

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;

		//Creates a null-vector 
		Vector2<T>();

		//Creates a vector (aX, aY, aZ) 
		Vector2<T>(const T& aX, const T& aY);

		//Copy constructor (compiler generated) 
		Vector2<T>(const Vector2<T>& aVector) = default;

		//Assignment operator (compiler generated) 
		Vector2<T>& operator=(const Vector2<T>& aVector3) = default;

		//Destructor (compiler generated) 
		~Vector2<T>() = default;

		//Returns the squared length of the vector 
		T LengthSqr() const;

		//Returns the length of the vector 
		T Length() const;

		//Returns a normalized copy of this 
		Vector2<T> GetNormalized() const;

		//Normalizes the vector 
		void Normalize();

		//Returns the dot product of this and aVector 
		T Dot(const Vector2<T>& aVector) const;


		void Print() const;

		void DebugPrint() const;
		bool operator==(const Vector2<T>& vector2) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& obj)
		{
			os << "X: " << obj.x << " Y: " << obj.y << " Z: " << std::endl;
			return os;
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& aOut, const Vector2<T>& aVector3)
	{
		aOut << "(X: " << aVector3.x << " | Y: " << aVector3.y << ")";
		return aOut;
	}

	template <class T>
	Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}

	template <class T>
	Vector2<T>::Vector2(const T&aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template <class T>
	T Vector2<T>::LengthSqr() const
	{
		return ((x * x) + (y * y));
	}

	template <class T>
	T Vector2<T>::Length() const
	{
		return sqrt((x * x) + (y * y));
	}

	template <class T>
	Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();

		assert(length != 0 && "Length is 0");

		if (length != 0.0f)
		{
			T lengthInverse = 1 / length;
			return Vector2<T>(x * lengthInverse, y * lengthInverse);
		}

		return Vector2<T>(x * 0.0f, y * 0.0f);
	}

	template <class T>
	void Vector2<T>::Normalize()
	{
		assert(x != 0 && "X is 0 and cant be normilized");
		assert(y != 0 && "X is 0 and cant be normilized");

		Vector2<T> normalized = GetNormalized();

		x = normalized.x;
		y = normalized.y;

	}

	template <class T>
	T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return (x * aVector.x) + (y * aVector.y);
	}

	template <class T>
	void Vector2<T>::Print() const
	{
		std::cout << "X:\t" << x << "\tY:\t" << y << std::endl;
	}

	template <class T>
	void Vector2<T>::DebugPrint() const
	{
		std::cout <<
			"X: " << x <<
			"\tY: " << y <<
			"\tSize:\t" << sizeof(*this) <<
			std::endl;
	}

	template <class T>
	bool Vector2<T>::operator==(const Vector2& vector2) const
	{
		return x == vector2.x && y == vector2.y;
	}

	//Returns the vector sum of aVector0 and aVector1 
	template <class T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	//Returns the vector difference of aVector0 and aVector1 
	template <class T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	//Returns the vector aVector multiplied by the scalar aScalar 
	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar 
	template <class T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar == 0 && "Scaler can't be 0 since you cant divide by 0");

		return Vector2<T>(aVector.x / aScalar, aVector.y / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1) 
	template <class T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1) 
	template <class T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	//Equivalent to setting aVector to (aVector * aScalar) 
	template <class T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar) 
	template <class T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Scaler can't be 0 since you cant divide by 0");

		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}

}


