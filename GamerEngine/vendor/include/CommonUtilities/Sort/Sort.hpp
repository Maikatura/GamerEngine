#pragma once

#include <vector>



namespace CommonUtilities
{
    template <class T>
    void SelectionSort(std::vector<T>& aVector)
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

    template <class T>
    void BubbleSort(std::vector<T>& aVector)
    {
	    const int vectorSize = aVector.size();

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

    template <class T>
    void MergeSort(std::vector<T>& aVector)
    {
        MergeSort(aVector, 0, aVector.size() - 1);
    }

    template<class T>
    void QuickSort(std::vector<T>& aVector)
    {
        int low = 0;
        int high = aVector.size() - 1;

        RealQuickSort(aVector, low, high);
    }

    // DONT USE THE BELOW ONES

    template <class T>
    int QuickSort(std::vector<T>& aVector, int aLow, int aHigh) {
        // pivot (Element to be placed at right position)
        int pivot = aVector[aHigh];

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
    void RealQuickSort(std::vector<T>& aVector, int aLow, int aHigh)
    {
        if (aLow < aHigh)
        {
            T pivot = QuickSort(aVector, aLow, aHigh);

            RealQuickSort(aVector, aLow, pivot - 1);
            RealQuickSort(aVector, pivot + 1, aHigh);
        }
    }

    template<class T>
    void MergeSort(std::vector<T>& aVector, int aLeft, int aMiddle, int aRight)
    {
        auto const subArrayOne = aMiddle - aLeft + 1;
        auto const subArrayTwo = aRight - aMiddle;

        auto* leftArray = new int[subArrayOne],
            * rightArray = new int[subArrayTwo];

        for (auto i = 0; i < subArrayOne; i++)
        {
            leftArray[i] = aVector[aLeft + i];
        }
        for (auto j = 0; j < subArrayTwo; j++)
        {
            rightArray[j] = aVector[aMiddle + 1 + j];
        }

        auto indexOfSubArrayOne = 0;
        auto indexOfSubArrayTwo = 0;
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
    }

    template <class T>
    void MergeSort(std::vector<T>& aVector, const int aBegin, const int aEnd)
    {
        if (aBegin >= aEnd)
            return;

        auto mid = aBegin + (aEnd - aBegin) / 2;
        MergeSort(aVector, aBegin, mid);
        MergeSort(aVector, mid + 1, aEnd);
        MergeSort(aVector, aBegin, mid, aEnd);
    }

}
