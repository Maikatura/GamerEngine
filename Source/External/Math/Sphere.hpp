#pragma once
#include "Vector.h"

namespace CommonUtilities
{


	template<typename T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();

		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);

		// Constructor that takes the center position and radius of the sphere.
		Sphere(const CommonUtilities::Vector3<T>& aCenter, T aRadius);

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		T myRadius;
		Vector3<T> myPosition;


	};

	
}

template <typename T>
CommonUtilities::Sphere<T>::Sphere()
{
}

template <typename T>
CommonUtilities::Sphere<T>::Sphere(const Sphere<T>& aSphere)
{
}

template <typename T>
CommonUtilities::Sphere<T>::Sphere(const CommonUtilities::Vector3<T>& aCenter, T aRadius)
{
	myPosition = aCenter;
	myRadius = aRadius;
}

template <typename T>
void CommonUtilities::Sphere<T>::InitWithCenterAndRadius(const CommonUtilities::Vector3<T>& aCenter, T aRadius)
{
	myPosition = aCenter;
	myRadius = aRadius;
}

template <typename T>
bool CommonUtilities::Sphere<T>::IsInside(const Vector3<T>& aPosition) const
{

	Vector3<T> diff = myPosition - aPosition;

	if (diff.Length() > myRadius)
	{
		return false;
	}

	return true;
}
