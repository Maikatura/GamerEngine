#pragma once
#include "Vector.h"
namespace CommonUtilities
{

	template<typename T>
	class Ray
	{

	public:
		// Default constructor: there is no ray, the origin and direction are the
		// zero vector.
		Ray();

		// Copy constructor.
		Ray(const Ray<T>& aRay);

		// Constructor that takes two points that define the ray, the direction is
		// aPoint - aOrigin and the origin is aOrigin.
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};
}

template <typename T>
CommonUtilities::Ray<T>::Ray()
{
}

template <typename T>
CommonUtilities::Ray<T>::Ray(const Ray<T>& aRay)
{
	myOrigin = aRay.myOrigin;
	myDirection = aRay.myDirection;
}

template <typename T>
CommonUtilities::Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
{
	myOrigin = aOrigin;
	myDirection = (aOrigin - aPoint).GetNormalized();
}

template <typename T>
void CommonUtilities::Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
{
	myOrigin = aOrigin;
	myDirection = (aOrigin - aPoint).GetNormalized();
}

template <typename T>
void CommonUtilities::Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
{
	myOrigin = aOrigin;
	myDirection = aDirection;
}
