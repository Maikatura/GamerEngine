#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();

		// Copy constructor.
		Line(const Line <T>& aLine);

		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T>& aPosition) const;

		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T>& GetNormal() const;

	private:
		Vector2<T> myPoint0;
		Vector2<T> myPoint1;
		Vector2<T> myDirection;
		Vector2<T> myNormal;
	};

	template <typename T>
	Line<T>::Line()
	{
		myPoint0 = Vector2<T>(0, 0);
		myPoint1 = Vector2<T>(0, 0);
	}

	template <typename T>
	Line<T>::Line(const Line<T>& aLine)
	{
		*this = aLine;
	}

	template <typename T>
	Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;

		myDirection.x = aPoint1.x - aPoint0.x;
		myDirection.y = aPoint1.y - aPoint0.y;
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	template <typename T>
	void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPoint0 = aPoint0;
		myPoint1 = aPoint1;

		myDirection.x = aPoint1.x - aPoint0.x;
		myDirection.y = aPoint1.y - aPoint0.y;
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	template <typename T>
	void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint0 = aPoint;
		myPoint1 = aPoint;
		myDirection = aDirection;
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	template <typename T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> direction = GetDirection();

		Vector2<T> normal = GetNormal();


		direction.x = myPoint1.x - myPoint0.x;
		direction.y = myPoint1.y - myPoint0.y;


		T lineToPointX = aPosition.x - myPoint0.x;
		T lineToPointY = aPosition.y - myPoint0.y;

		T dotLineToPoint = (lineToPointX * normal.x) + (lineToPointY * normal.y);

		if (dotLineToPoint < 0.000001f)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}

	template <typename T>
	const Vector2<T>& Line<T>::GetNormal() const
	{
		return myNormal;
	}
}