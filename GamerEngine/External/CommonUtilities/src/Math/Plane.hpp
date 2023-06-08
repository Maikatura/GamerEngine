#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Plane
	{
	public:
		// Default constructor.
		Plane();
		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 - aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);

		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T>& aPosition) const;

		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const;

		const Vector3<T>& GetCenter() const;

		T GetSignedDistanceToPlane(const Vector3<T>& aPoint) const;
		T GetDistance() const;

	private:
		Vector3<T> myPoint0;
		Vector3<T> myPoint1;
		Vector3<T> myPoint2;
		Vector3<T> myDirection;
		Vector3<T> myNormal;
	};

	template <typename T>
	Plane<T>::Plane()
	{
	}

	template <typename T>
	Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;
		myPoint2 = aPoint2;



		T vec1X = aPoint1.x - aPoint0.x;
		T vec1Y = aPoint1.y - aPoint0.y;
		T vec1Z = aPoint1.z - aPoint0.z;

		T vec2X = aPoint2.x - aPoint0.x;
		T vec2Y = aPoint2.y - aPoint0.y;
		T vec2Z = aPoint2.z - aPoint0.z;

		myNormal.x = (vec1Y * vec2Z) - (vec1Z * vec2Y);
		myNormal.y = (vec1Z * vec2X) - (vec1X * vec2Z);
		myNormal.z = (vec1X * vec2Y) - (vec1Y * vec2X);

		

	}

	template <typename T>
	Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint0;
		myPoint2 = aPoint0;
		myNormal = aNormal;
	}

	template <typename T>
	void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;
		myPoint2 = aPoint2;


		T vec1X = myPoint1.x - myPoint0.x;
		T vec1Y = myPoint1.y - myPoint0.y;
		T vec1Z = myPoint1.z - myPoint0.z;

		T vec2X = myPoint2.x - myPoint0.x;
		T vec2Y = myPoint2.y - myPoint0.y;
		T vec2Z = myPoint2.z - myPoint0.z;

		myNormal.x = (vec1Y * vec2Z) - (vec1Z * vec2Y);
		myNormal.y = (vec1Z * vec2X) - (vec1X * vec2Z);
		myNormal.z = (vec1X * vec2Y) - (vec1Y * vec2X);
	}

	template <typename T>
	void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint0 = aPoint;
		myPoint1 = aPoint;
		myPoint2 = aPoint;
		myNormal = aNormal;
	}

	template <typename T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{

		Vector3<T> normal = GetNormal();

		T vec1X = myPoint1.x - myPoint0.x;
		T vec1Y = myPoint1.y - myPoint0.y;
		T vec1Z = myPoint1.z - myPoint0.z;

		T vec2X = myPoint2.x - myPoint0.x;
		T vec2Y = myPoint2.y - myPoint0.y;
		T vec2Z = myPoint2.z - myPoint0.z;

		T planeToLineX = aPosition.x - myPoint0.x;
		T planeToLineY = aPosition.y - myPoint0.y;
		T planeToLineZ = aPosition.z - myPoint0.z;

		T dotPlaneToLine = (planeToLineX * normal.x) + (planeToLineY * normal.y) + (planeToLineZ * normal.z);

		if (dotPlaneToLine < 0.000001f)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	const Vector3<T>& Plane<T>::GetNormal() const
	{
		return myNormal;
	}

	template <typename T>
	const Vector3<T>& Plane<T>::GetCenter() const
	{
		return myPoint0;
	}

	template <typename T>
	T Plane<T>::GetSignedDistanceToPlane(const Vector3<T>& aPoint) const
	{
		return Vector3<T>::Dot(myNormal, aPoint) - GetDistance();
	}

	template <typename T>
	T Plane<T>::GetDistance() const
	{
		return Vector3<T>::Dot(myNormal, myPoint0);
	}
}
