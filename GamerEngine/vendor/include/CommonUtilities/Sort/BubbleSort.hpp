#pragma once 
#include <vector>

namespace CommonUtilities
{
	template <class T>
	void BubbleSort(std::vector<T>& aVector);
}

template<class T>
void CommonUtilities::BubbleSort(std::vector<T>& aVector)
{
	int vectorSize = aVector.size();

	for (int i = 0; i < vectorSize - 1; i++)
	{
		for (int j = 0; j < vectorSize - i - 1; j++)
		{
			if (aVector[j + 1] < aVector[j])
			{
				std::swap(aVector[j], aVector[j + 1]);
			}
		}
	}
}