#pragma once
#include <cassert>


namespace CommonUtilities
{
	template<typename T>
	T Distance(const T aFirst, const T aSecond)
	{
		return Abs(aFirst - aSecond);
	}

	template<typename T>
	T Max(const T aFirst, const T aSecond)
	{
		if (aFirst >= aSecond)
		{
			return aFirst;
		}
		else
		{
			return aSecond;
		}
	}

	template <typename T>
	T Min(const T aFirst, const T aSecond)
	{
		if (aFirst <= aSecond)
		{
			return aFirst;
		}
		else
		{
			return aSecond;
		}
	}


	template <typename T>
	T Abs(const T aAbsValue)
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
	T Clamp(T aClampValue, const T aMinValue, const T aMaxValue)
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
	T Lerp(const T aMinValue, const T aMaxValue, const float aLerp)
	{
		T returnValue = aMinValue + (aLerp * (aMaxValue - aMinValue));

		return returnValue;
	}

	template <typename T>
	void Swap(T& aFirstValue, T& aSecondValue)
	{
		T valueOne = aFirstValue;
		T valueTwo = aSecondValue;

		aSecondValue = valueOne;
		aFirstValue = valueTwo;
	}

	template <typename T>
	T Center(const T aFirst, const T aSecond)
	{
		T center = aFirst - aSecond;

		return aFirst + center;
	}

};