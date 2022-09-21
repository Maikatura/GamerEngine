#pragma once
#include "DoublyLinkedListNode.hpp"

namespace CommonUtilities
{
	template <class T>
	class DoublyLinkedList
	{
	public:
		DoublyLinkedList();
		~DoublyLinkedList();
		int GetSize() const;
		DoublyLinkedListNode<T>* GetFirst();
		DoublyLinkedListNode<T>* GetLast();
		void InsertFirst(const T& aValue);
		void InsertLast(const T& aValue);
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);
		void Remove(DoublyLinkedListNode<T>* aNode);
		DoublyLinkedListNode<T>* FindFirst(const T& aValue);
		DoublyLinkedListNode<T>* FindLast(const T& aValue);
		bool RemoveFirst(const T& aValue);
		bool RemoveLast(const T& aValue);

	private:
		DoublyLinkedListNode<T>* myFirstNode;
		DoublyLinkedListNode<T>* myLastNode;
	};

}
template <class T>
CommonUtilities::DoublyLinkedList<T>::DoublyLinkedList()
{
	myFirstNode = nullptr;
	myLastNode = nullptr;
}

template <class T>
CommonUtilities::DoublyLinkedList<T>::~DoublyLinkedList()
{

}

template <class T>
int CommonUtilities::DoublyLinkedList<T>::GetSize() const
{
	int size = 0;
	CommonUtilities::DoublyLinkedListNode<T>* findNode = myFirstNode;

	if (findNode != nullptr)
	{
		size = 1;
		while (findNode->GetNext() != nullptr)
		{
			findNode = findNode->GetNext();
			size++;
		}
	}

	return size;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedList<T>::GetFirst()
{
	return myFirstNode;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedList<T>::GetLast()
{
	return myLastNode;
}

template <class T>
void CommonUtilities::DoublyLinkedList<T>::InsertFirst(const T& aValue)
{
	CommonUtilities::DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

	newNode->myNext = myFirstNode;
	newNode->myPrevious = nullptr;
	if (myFirstNode != nullptr)
	{
		myFirstNode->myPrevious = newNode;
	}



	myFirstNode = newNode;

	CommonUtilities::DoublyLinkedListNode<T>* temp = myFirstNode;
	while (temp->myNext != NULL)
	{
		temp = temp->myNext;
	}
	myLastNode = temp;
}

template <class T>
void CommonUtilities::DoublyLinkedList<T>::InsertLast(const T& aValue)
{
	CommonUtilities::DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

	newNode->myNext = nullptr;
	newNode->myPrevious = myLastNode;
	if (myFirstNode != nullptr)
	{
		myLastNode->myNext = newNode;
	}



	myLastNode = newNode;

	
	if (myFirstNode != nullptr)
	{
		CommonUtilities::DoublyLinkedListNode<T>* temp = myFirstNode;

		while (temp->myPrevious != NULL)
		{
			temp = temp->myPrevious;
		}

		myFirstNode = temp;
	}
	else
	{
		myFirstNode = newNode;
	}
	

}

template <class T>
void CommonUtilities::DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
{
	CommonUtilities::DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

	auto temp = aNode->myPrevious;
	aNode->myPrevious = newNode;
	newNode->myNext = aNode;
	newNode->myPrevious = temp;
}

template <class T>
void CommonUtilities::DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
{
	CommonUtilities::DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

	auto temp = aNode->myNext;
	aNode->myNext = newNode;
	newNode->myNext = temp;
	newNode->myPrevious = aNode;
}

template <class T>
void CommonUtilities::DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
{
	if (aNode == myFirstNode)
	{
		myFirstNode = aNode->myNext;
		myFirstNode->myPrevious = nullptr;
	}
	else if (aNode == myLastNode)
	{
		myLastNode = aNode->myPrevious;
		myLastNode->myNext = nullptr;
	}
	else
	{
		auto temp1 = aNode->myPrevious;
		auto temp2 = aNode->myNext;

		temp1->myNext = temp2;
		temp2->myPrevious = temp1;
	}
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedList<T>::FindFirst(const T& aValue)
{
	auto temp = myFirstNode;
	while (temp->GetValue() != aValue)
	{
		temp = temp->myNext;
	}

	return temp;
}

template <class T>
CommonUtilities::DoublyLinkedListNode<T>* CommonUtilities::DoublyLinkedList<T>::FindLast(const T& aValue)
{
	auto temp = myLastNode;
	while (temp->GetValue() != aValue)
	{
		temp = temp->myPrevious;
	}

	return temp;
}

template <class T>
bool CommonUtilities::DoublyLinkedList<T>::RemoveFirst(const T& aValue)
{
	auto temp = FindFirst(aValue);
	if (temp != nullptr)
	{
		Remove(temp);
		return true;
	}

	return false;
}

template <class T>
bool CommonUtilities::DoublyLinkedList<T>::RemoveLast(const T& aValue)
{
	auto temp = FindLast(aValue);
	if (temp != nullptr)
	{
		Remove(temp);
		return true;
	}


	return false;

}
