#pragma once 
#include <vector>

namespace CommonUtilities 
{
	template <class T>
	void SelectionSort(std::vector<T>& aVector);
}

template<class T>
void CommonUtilities::SelectionSort(std::vector<T>& aVector)
{
    int minValueIndex;
    int vectorSize = aVector.size();

    for (int i = 0; i < vectorSize - 1; i++)
    {
        minValueIndex = i;
        for (int j = i + 1; j < vectorSize; j++) 
        {
            if (aVector[j] < aVector[minValueIndex]) 
            {
                minValueIndex = j;
            }
        }
           
        std::swap(aVector[minValueIndex], aVector[i]);
    }
}