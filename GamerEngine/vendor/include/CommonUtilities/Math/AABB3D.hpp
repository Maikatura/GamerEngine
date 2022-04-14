#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	template<typename T>
	class AABB3D
	{
	public:
		// Default constructor : there is no AABB, both minand max points are the zero vector.
		AABB3D();

		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);

		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> myMinPos;
		Vector3<T> myMaxPos;
	};
}

template <typename T>
CommonUtilities::AABB3D<T>::AABB3D()
{
	myMinPos = Vector3<T>(0, 0);
	myMaxPos = Vector3<T>(0, 0);
}

template <typename T>
CommonUtilities::AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
{
	this = aAABB3D;
}

template <typename T>
CommonUtilities::AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
{
	myMinPos = aMin;
	myMaxPos = aMax;
}

template <typename T>
void CommonUtilities::AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
{
	myMinPos = aMin;
	myMaxPos = aMax;
}

template <typename T>
bool CommonUtilities::AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
{
	if (aPosition.x < myMinPos.x || aPosition.x > myMaxPos.x) return false;
	if (aPosition.y < myMinPos.y || aPosition.y > myMaxPos.y) return false;
	if (aPosition.z < myMinPos.z || aPosition.z > myMaxPos.z) return false;

	return true;
}
