#pragma once
#include <vector>

namespace CommonUtilities
{
	
	template<class T>
	void BogoSort(std::vector<T>& aVector);
	
	template<class T>
	void Shuffle(std::vector<T>& aVector);
	
	template<class T>
	bool IsSorted(std::vector<T>& aVector);
}

template<class T>
void CommonUtilities::BogoSort(std::vector<T>& aVector)
{
	while (!IsSorted(aVector))
	{
		Shuffle(aVector);
	}
}

template<class T>
void CommonUtilities::Shuffle(std::vector<T>& aVector)
{
	for (int i = 0; i < aVector.size(); i++)
	{
        std::swap(aVector[i], aVector[rand() % aVector.size()]);
	}
}

template<class T>
bool CommonUtilities::IsSorted(std::vector<T>& aVector)
{
	int n = aVector.size();
    while ( --n > 1 )
	{
		if (aVector[n] < aVector[n-1])
		{
			return false;
		}
	}
        
    return true;
}