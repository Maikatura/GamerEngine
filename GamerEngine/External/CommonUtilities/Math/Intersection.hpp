#pragma once
#include "Plane.hpp"
#include "AABB3D.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"

namespace CommonUtilities
{
	template <typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay,
		Vector3<T>& aOutIntersectionPoint)
	{
		float denom = aPlane.GetNormal().Dot(aRay.myDirection);
		if (abs(denom) > 0.0001f)
		{
			float t = (aRay.myOrigin).Dot(aPlane.GetNormal()) / denom;
			aOutIntersectionPoint = aPlane.GetNormal() + aRay.myDirection;
			if (t >= 0)
			{
				return true;
			}
		}

		aOutIntersectionPoint = Vector3<T>();
		return false;
	}

	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		float xt, xn;
		if (aRay.myOrigin.x < aAABB.myMinPos.x)
		{
			xt = aAABB.myMinPos.x - aRay.myOrigin.x;

			if (xt > aRay.myDirection.x)
			{
				return false;
			}

			xt /= aRay.myDirection.x;
			xn = -1.0f;
		}
		else if (aRay.myOrigin.x > aAABB.myMaxPos.x)
		{
			xt = aAABB.myMaxPos.x - aRay.myOrigin.x;

			if (xt < aRay.myDirection.x)
			{
				return false;
			}

			xt /= aRay.myDirection.x;
			xn = 1.0f;
		}
		else
		{
			xt = -1.0f;
		}

		if (aRay.myOrigin.x < aAABB.myMinPos.x)
		{
			xt = aAABB.myMinPos.x - aRay.myOrigin.x;

			if (xt > aRay.myDirection.x)
			{
				return false;
			}

			xt /= aRay.myDirection.x;
			xn = -1.0f;
		}
		else if (aRay.myOrigin.x > aAABB.myMaxPos.x)
		{
			xt = aAABB.myMaxPos.x - aRay.myOrigin.x;

			if (xt < aRay.myDirection.x)
			{
				return false;
			}

			xt /= aRay.myDirection.x;
			xn = 1.0f;
		}
		else
		{
			xt = -1.0f;
		}

		if (aRay.myOrigin.y < aAABB.myMinPos.y)
		{
			xt = aAABB.myMinPos.y - aRay.myOrigin.y;

			if (xt > aRay.myDirection.y)
			{
				return false;
			}

			xt /= aRay.myDirection.y;
			xn = -1.0f;
		}
		else if (aRay.myOrigin.y > aAABB.myMaxPos.y)
		{
			xt = aAABB.myMaxPos.y - aRay.myOrigin.y;

			if (xt < aRay.myDirection.y)
			{
				return false;
			}

			xt /= aRay.myDirection.y;
			xn = 1.0f;
		}
		else
		{
			xt = -1.0f;
		}

		if (aRay.myOrigin.z < aAABB.myMinPos.z)
		{
			xt = aAABB.myMinPos.z - aRay.myOrigin.z;

			if (xt > aRay.myDirection.z)
			{
				return false;
			}

			xt /= aRay.myDirection.z;
			xn = -1.0f;
		}
		else if (aRay.myOrigin.z > aAABB.myMaxPos.z)
		{
			xt = aAABB.myMaxPos.z - aRay.myOrigin.z;

			if (xt < aRay.myDirection.z)
			{
				return false;
			}

			xt /= aRay.myDirection.z;
			xn = 1.0f;
		}
		else
		{
			xt = -1.0f;
		}

		return true;
	}

	template <typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> minusPosition = aRay.myOrigin - aSphere.myPosition;

		float p = aRay.myDirection.Dot(minusPosition);
		float q = minusPosition.Dot(minusPosition) - (aSphere.myRadius * aSphere.myRadius);

		float discriminant = (p * p) - q;
		if (discriminant < 0.0f)
		{
			return false;
		}

		return true;
	}
}
