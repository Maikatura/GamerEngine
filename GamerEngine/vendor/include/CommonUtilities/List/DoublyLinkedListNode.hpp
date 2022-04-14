#pragma once


namespace CommonUtilities
{
	template<class T>
	class DoublyLinkedList;

	template<class T>
	class DoublyLinkedListNode
	{
	public:
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;
		const T& GetValue() const;
		T& GetValue();
		DoublyLinkedListNode<T>* GetNext() const;
		DoublyLinkedListNode<T>* GetPrevious() const;
	private:
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode() {}

		T myValue;

		DoublyLinkedListNode<T>* myPrevious;
		DoublyLinkedListNode<T>* myNext;
	};

	
}

template <class T>
const T& CommonUtilities::DoublyLinkedListNode<T>::GetValue() const
{
	return myValue;
}

template <class T>
T& CommonUtilities::DoublyLinkedListNode<T>::GetValue()
{
	return myValue;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedListNode<T>::GetNext() const
{
	return myNext;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedListNode<T>::GetPrevious() const
{
	return myPrevious;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
{
	myValue = aValue;
}






