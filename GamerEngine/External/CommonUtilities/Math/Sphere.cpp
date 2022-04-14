#include "pch.h"
#include "Sphere.h"

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
	// from s1 to s2
	Vector3<T> pDiff = myPosition - aPosition;
	// collision detection

	if (pDiff.Length() >= aPosition.x + myRadius &&
		pDiff.Length() >= aPosition.y + myRadius &&
		pDiff.Length() >= aPosition.z + myRadius)
	{
		return false;
	}

	return true;
}
