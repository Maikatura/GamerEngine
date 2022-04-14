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

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};
}