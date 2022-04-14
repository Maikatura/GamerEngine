#pragma once
#include <vector>
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom k�
		Queue();
		//Returnerar antal element i k�n
		int GetSize() const;
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		const T& GetFront() const;
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		bool IsEmpty();
		T& GetFront();
		//L�gger in ett nytt element l�ngst bak i k�n
		void Enqueue(const T& aValue);
		//Tar bort elementet l�ngst fram i k�n och returnerar det. Kraschar med en
		//assert om k�n �r tom.
		T Dequeue();
	private:
		int myFirstIndex;
		int myLastIndex;
		int myNormalSize;
		int myScaleValue;
		std::vector<T> myQueueVector;
	};

	template <class T>
	Queue<T>::Queue()
	{
		myFirstIndex = 0;
		myLastIndex = 0;
		myNormalSize = 16;
		myScaleValue = 1;
		myQueueVector.reserve(myNormalSize);
	}

	template <class T>
	int Queue<T>::GetSize() const
	{
		return myLastIndex - myFirstIndex;
	}

	template <class T>
	const T& Queue<T>::GetFront() const
	{
		assert(myLastIndex - myFirstIndex < 0 && "Value out of range");
		return myQueueVector[myFirstIndex];
	}

	template <class T>
	T& Queue<T>::GetFront()
	{
		assert(myLastIndex - myFirstIndex < 0 && "Value out of range");
		return myQueueVector[myFirstIndex];

		
	}

	template <class T>
	void Queue<T>::Enqueue(const T& aValue)
	{
		myQueueVector.push_back(aValue);
		myLastIndex++;

		if (myLastIndex - myFirstIndex > myNormalSize * myScaleValue)
		{
			myScaleValue++;
			myQueueVector.reserve(myNormalSize * myScaleValue);
		}
	}

	template <class T>
	T Queue<T>::Dequeue()
	{
		if (GetSize() > 0)
		{
			myFirstIndex++;
			return myQueueVector[myFirstIndex - 1];
		}

		return NULL;
	}


	template <class T>
	bool Queue<T>::IsEmpty()
	{
		return (myLastIndex == myFirstIndex);
	}
}
