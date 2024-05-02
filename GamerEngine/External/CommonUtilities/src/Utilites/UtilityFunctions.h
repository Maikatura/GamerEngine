#pragma once
#include <cassert>



#define ToInt(a) static_cast<int>(a)

namespace CommonUtilities
{
	const float PI = 3.141f;
	const float DegToRad = static_cast<float>(PI / 180.0f);
	const float RadToDeg = static_cast<float>(180.0f / PI);

	template<typename T>
	inline T Distance(const T aFirst, const T aSecond)
	{
		return Abs(aFirst - aSecond);
	}

	template<typename T>
	inline T Max(const T aFirst, const T aSecond)
	{
		return ((aFirst > aSecond) ? aFirst : aSecond);
	}

	template <typename T>
	inline T Min(const T aFirst, const T aSecond)
	{
		return (aFirst < aSecond) ? aFirst : aSecond;
	}


	template <typename T>
	inline T Abs(const T aAbsValue)
	{
		if (aAbsValue < 0 || aAbsValue == 0)
		{
			return abs(aAbsValue);
		}
		else
		{
			return aAbsValue;
		}
	}

	template <typename T>
	inline T Clamp(T aClampValue, const T aMinValue, const T aMaxValue)
	{
		assert(aMinValue <= aMaxValue && "ERROR! min value is larger than max value");

		if (aClampValue <= aMinValue)
		{
			aClampValue = aMinValue;
		}
		else if (aClampValue >= aMaxValue)
		{
			aClampValue = aMaxValue;
		}

		return aClampValue;
	}

	template <typename T>
	inline T Lerp(const T aMinValue, const T aMaxValue, const float aLerp)
	{
		T returnValue = aMinValue + (aLerp * (aMaxValue - aMinValue));

		return returnValue;
	}

	template <typename T>
	inline T LerpMinMax(const T aMinValue, const T aMaxValue, const float aLerp)
	{
		return aMinValue + (aMaxValue - aMinValue) * (Min<float>(Max<float>(aLerp, 0), 1.f));
	}

	template <typename T>
	inline void Swap(T& aFirstValue, T& aSecondValue)
	{
		T valueOne = aFirstValue;
		T valueTwo = aSecondValue;

		aSecondValue = valueOne;
		aFirstValue = valueTwo;
	}

	template <typename T>
	inline T Center(const T aFirst, const T aSecond)
	{
		T center = aFirst - aSecond;

		return aFirst + center;
	}

	template <typename T>
	inline bool Equals(const T& a, const T& b, const T& epsilon)
	{
		return std::abs(a - b) <= epsilon;
	}

	template<class T>
	inline bool AreEqual(T a, T b, T tolerance = 1e-5)
	{
		return std::fabs(a - b) < tolerance;
	}

	template<class T>
	inline bool IsGreaterThan(T a, T b, T tolerance = 1e-5)
	{
		return (a - b) > tolerance;
	}

	template<class T>
	inline bool IsLessThan(T a, T b, T tolerance = 1e-5)
	{
		return (b - a) > tolerance;
	}
};