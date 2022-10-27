#pragma once 
#include <vector>

namespace CommonUtilities
{
	template <class T>
	void MergeSort(std::vector<T>& aVector);
	/**
	 * \brief DONT USE
	 * \tparam T 
	 * \param aVector 
	 * \param aLeft 
	 * \param aMiddle 
	 * \param aRight 
	 */
	template <class T>
    void MergeSort(std::vector<T>& aVector, int aLeft, int aMiddle, int aRight);


	/**
	 * \brief DONT USE
	 * \tparam T 
	 * \param aVector 
	 * \param aBegin 
	 * \param aEnd 
	 */
	template <class T>
    void MergeSort(std::vector<T>& aVector, const int aBegin, const int aEnd);
}

template <class T>
void CommonUtilities::MergeSort(std::vector<T>& aVector)
{
    MergeSort(aVector, 0, aVector.size() - 1);
}
 
template<class T>
void CommonUtilities::MergeSort(std::vector<T>& aVector, int aLeft, int aMiddle, int aRight)
{
    auto const subArrayOne = aMiddle - aLeft + 1;
    auto const subArrayTwo = aRight - aMiddle;

    T* leftArray = new T[subArrayOne];
	T* rightArray = new T[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++)
    {
        leftArray[i] = aVector[aLeft + i];
    }
    for (auto j = 0; j < subArrayTwo; j++)
    {
        rightArray[j] = aVector[aMiddle + 1 + j];
    }

    int indexOfSubArrayOne = 0;
    int indexOfSubArrayTwo = 0;
    int indexOfMergedArray = aLeft;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) 
    {
        if (leftArray[indexOfSubArrayOne] < rightArray[indexOfSubArrayTwo]) 
        {
            aVector[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else 
        {
            aVector[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) 
    {
        aVector[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo)
    {
        aVector[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

template <class T>
void CommonUtilities::MergeSort(std::vector<T>& aVector, const int aBegin, const int aEnd)
{
    if (aBegin >= aEnd)
        return;

    auto mid = aBegin + (aEnd - aBegin) / 2;
    MergeSort(aVector, aBegin, mid);
    MergeSort(aVector, mid + 1, aEnd);
    MergeSort(aVector, aBegin, mid, aEnd);
}
