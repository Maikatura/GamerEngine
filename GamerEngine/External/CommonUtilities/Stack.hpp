#pragma once
#include <vector>

namespace CommonUtilities
{
	template<class T>
	class Stack
	{
	public:
		Stack();
		int GetSize() const;
		const T& GetTop() const;
		T& GetTop();
		void Push(const T& aValue);
		T Pop();
	private:
		std::vector<T> myStackVector;
	};

	template <class T>
	Stack<T>::Stack()
	{
		myStackVector.reserve(16);
	}

	template <class T>
	int Stack<T>::GetSize() const
	{
		return myStackVector.size();
	}

	template <class T>
	const T& Stack<T>::GetTop() const
	{
		return myStackVector[0];
	}

	template <class T>
	T& Stack<T>::GetTop()
	{
		return myStackVector[0];
	}

	template <class T>
	void Stack<T>::Push(const T& aValue)
	{
		myStackVector.push_back(aValue);
	}

	template <class T>
	T Stack<T>::Pop()
	{
		if (myStackVector.size() > 0)
		{
			T value = myStackVector[myStackVector.size() - 1];
			myStackVector.pop_back();
			return value;
		}
		
		return NULL;
	}
}
