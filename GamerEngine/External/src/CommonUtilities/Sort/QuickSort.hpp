#pragma once 
#include <vector>

namespace CommonUtilities
{
	template <class T>
	void QuickSort(std::vector<T>& aVector);

	template <class T>
	int QuickSort(std::vector<T>& aVector, int aLow, int aHigh);

	template <class T>
	void RealQuickSort(std::vector<T>& aVector, int aLow, int aHigh);
}

template<class T>
void CommonUtilities::QuickSort(std::vector<T>& aVector)
{
	int low = 0;
	int high = aVector.size() - 1;

	RealQuickSort(aVector, low, high);
}

template <class T>
int CommonUtilities::QuickSort(std::vector<T>& aVector, int aLow, int aHigh) {
	// pivot (Element to be placed at right position)
	T pivot = aVector[aHigh];

	int i = (aLow - 1);

	for (int j = aLow; j <= aHigh - 1; j++)
	{
		// If current element is smaller than the pivot
		if (aVector[j] < pivot)
		{
			i++;
			std::swap(aVector[i], aVector[j]);
		}
	}
	std::swap(aVector[i + 1], aVector[aHigh]);
	return (i + 1);
}

template<class T>
void CommonUtilities::RealQuickSort(std::vector<T>& aVector, int aLow, int aHigh)
{
	if (aLow < aHigh)
	{
		int pivot = QuickSort(aVector, aLow, aHigh);

		RealQuickSort(aVector, aLow, pivot - 1);
		RealQuickSort(aVector, pivot + 1, aHigh);
	}
}
