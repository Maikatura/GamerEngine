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

		friend std::ostream& operator<<(std::ostream& os, const Ray<T>& obj)
		{
			os << "Origin: " << myOrigin << " Direction: " << myDirection << std::endl;
			return os;
		}

		Vector3<T> GetOrigin() const { return myOrigin; }
		Vector3<T> GetDirection() const { return myDirection; }

	private:

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
	myDirection = (aOrigin - aPoint).Normalized();
}

template <typename T>
void CommonUtilities::Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
{
	myOrigin = aOrigin;
	myDirection = (aOrigin - aPoint).Normalized();
}

template <typename T>
void CommonUtilities::Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
{
	myOrigin = aOrigin;
	myDirection = aDirection;
}
