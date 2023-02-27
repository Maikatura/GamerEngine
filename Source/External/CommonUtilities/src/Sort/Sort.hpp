#pragma once

#include <vector>



namespace CommonUtilities
{

    // DONT USE THE BELOW ONES

    template <class T>
    int QuickSortWorker(std::vector<T>& aVector, int aLow, int aHigh) 
    {
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
    void RealQuickSort(std::vector<T>& aVector, int aLow, int aHigh)
    {
        if (aLow < aHigh)
        {
            int pivot = QuickSortWorker(aVector, aLow, aHigh);

            RealQuickSort(aVector, aLow, pivot - 1);
            RealQuickSort(aVector, pivot + 1, aHigh);
        }
    }

    template<class T>
    void RealMergeSort(std::vector<T>& aVector, int aLeft, int aMiddle, int aRight)
    {
    	const int subArrayOne = aMiddle - aLeft + 1;
    	const int subArrayTwo = aRight - aMiddle;

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
    void StartMergeSort(std::vector<T>& aVector, const int aBegin, const int aEnd)
    {
        if (aBegin >= aEnd)
            return;

        auto mid = aBegin + (aEnd - aBegin) / 2;
        StartMergeSort(aVector, aBegin, mid);
        StartMergeSort(aVector, mid + 1, aEnd);
        RealMergeSort(aVector, aBegin, mid, aEnd);
    }


    // Use this below

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

    
    template<class T>
    void QuickSort(std::vector<T>& aVector)
    {
        int low = 0;
        int high = aVector.size() - 1;

        RealQuickSort(aVector, low, high);
    }


    // MERGE SORT
    template<class T>
    void Merge(std::vector<T>& aVector, int const left, int const mid, int const right)
    {
        auto const subArrayOne = mid - left + 1;
        auto const subArrayTwo = right - mid;

        // Create temp arrays
        auto* leftArray = new T[subArrayOne],
            * rightArray = new T[subArrayTwo];

        // Copy data to temp arrays leftArray[] and rightArray[]
        for(auto i = 0; i < subArrayOne; i++)
            leftArray[i] = aVector[left + i];
        for(auto j = 0; j < subArrayTwo; j++)
            rightArray[j] = aVector[mid + 1 + j];

        auto indexOfSubArrayOne = 0, // Initial index of first sub-array
            indexOfSubArrayTwo = 0; // Initial index of second sub-array
        int indexOfMergedArray = left; // Initial index of merged array

        // Merge the temp arrays back into array[left..right]
        while(indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
        {
            if(leftArray[indexOfSubArrayOne] < rightArray[indexOfSubArrayTwo])
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
        // Copy the remaining elements of
        // left[], if there are any
        while(indexOfSubArrayOne < subArrayOne)
        {
            aVector[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }
        // Copy the remaining elements of
        // right[], if there are any
        while(indexOfSubArrayTwo < subArrayTwo)
        {
            aVector[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }

        delete[] leftArray;
        delete[] rightArray;
    }

    // begin is for left index and end is
    // right index of the sub-array
    // of arr to be sorted */
    template<class T>
    void MergeSort(std::vector<T>& aVector, int const begin, int const end)
    {
        if(begin >= end)
            return; // Returns recursively

        auto mid = begin + (end - begin) / 2;
        MergeSort(aVector, begin, mid);
        MergeSort(aVector, mid + 1, end);
        Merge(aVector, begin, mid, end);
    }

    template <class T>
    void MergeSort(std::vector<T>& aVector)
    {
        MergeSort(aVector, 0, static_cast<int>(aVector.size()) - 1);
    }


    // PTR

    template<class T>
    void RealMergeSortPtr(std::vector<T*>& aVector, int aLeft, int aMiddle, int aRight)
    {
        auto const subArrayOne = aMiddle - aLeft + 1;
        auto const subArrayTwo = aRight - aMiddle;

        T** leftArray = new T * [subArrayOne];
        T** rightArray = new T * [subArrayTwo];

        for(auto i = 0; i < subArrayOne; i++)
        {
            leftArray[i] = aVector[aLeft + i];
        }
        for(auto j = 0; j < subArrayTwo; j++)
        {
            rightArray[j] = aVector[aMiddle + 1 + j];
        }

        auto indexOfSubArrayOne = 0;
        auto indexOfSubArrayTwo = 0;
        int indexOfMergedArray = aLeft;

        while(indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
        {
            if(*leftArray[indexOfSubArrayOne] < *rightArray[indexOfSubArrayTwo])
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

        while(indexOfSubArrayOne < subArrayOne)
        {
            aVector[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }

        while(indexOfSubArrayTwo < subArrayTwo)
        {
            aVector[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }

        delete[] leftArray;
        delete[] rightArray;
    }

    template <class T>
    void StartMergeSortPtr(std::vector<T*>& aVector, const int aBegin, const int aEnd)
    {
        if(aBegin >= aEnd)
            return;

        auto mid = aBegin + (aEnd - aBegin) / 2;
        StartMergeSortPtr(aVector, aBegin, mid);
        StartMergeSortPtr(aVector, mid + 1, aEnd);
        RealMergeSortPtr(aVector, aBegin, mid, aEnd);
    }

    template <class T>
    void MergeSortPtr(std::vector<T*>& aVector)
    {
        StartMergeSortPtr(aVector, 0, static_cast<int>(aVector.size()) - 1);
    }


}