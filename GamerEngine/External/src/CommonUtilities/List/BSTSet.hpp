#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template<class T>
	struct TreeNode
	{
		T myValue;
		TreeNode<T>* myRightNode = nullptr;
		TreeNode<T>* myLeftNode = nullptr;
	};

	template <class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();
		//Returnerar true om elementet finns i mängden, och false annars.
		bool HasElement(const T& aValue);
		//Stoppar in elementet i mängden, om det inte redan finns där. Gör
		//ingenting annars.
		void Insert(const T& aValue);
		//Plockar bort elementet ur mängden, om det finns. Gör ingenting annars.
		void Remove(const T& aValue);

		void Print(const std::string& aPrefix = "", TreeNode<T>* aNode = nullptr, bool isLeft = false);

	private:
		void InsertElement(TreeNode<T>* aNode, const T& aValue);
		void RemoveElement(TreeNode<T>* aLastNode, TreeNode<T>* aNode, const T& aValue);
		bool FindElement(TreeNode<T>* aNode, const T& aValue);
		void DeleteElement(TreeNode<T>* aNode);

		TreeNode<T>* myFirstNode;
	};
}

template<class T>
CommonUtilities::BSTSet<T>::BSTSet()
{
	myFirstNode = nullptr;
}

template<class T>
CommonUtilities::BSTSet<T>::~BSTSet()
{
	/*DeleteElement(myFirstNode);
	myFirstNode = nullptr;*/
}

template<class T>
void CommonUtilities::BSTSet<T>::DeleteElement(TreeNode<T>* aNode)
{
	if (!aNode)
	{
		return;
	}

	if (aNode->myLeftNode)
	{
		DeleteElement(aNode->myLeftNode);
	}
	if (aNode->myRightNode)
	{
		DeleteElement(aNode->myRightNode);
	}

	delete aNode;
}

template<class T>
bool CommonUtilities::BSTSet<T>::HasElement(const T& aValue)
{
	return FindElement(myFirstNode, aValue);
}

template<class T>
bool CommonUtilities::BSTSet<T>::FindElement(TreeNode<T>* aNode, const T& aValue)
{
	bool left = false;
	bool right = false;


	if (aNode == nullptr)
	{
		return false;
	}

	if (!(aNode->myValue < aValue) && !(aValue < aNode->myValue))
	{
		return true;
	}

	auto tempLeft = aNode->myLeftNode;
	auto tempRight = aNode->myRightNode;

	if (tempLeft != NULL)
	{
		left = FindElement(tempLeft, aValue);
	}

	if (tempRight != NULL)
	{
		right = FindElement(tempRight, aValue);
	}


	if (left || right)
	{
		return true;
	}

	return false;
}

template<class T>
void CommonUtilities::BSTSet<T>::Insert(const T& aValue)
{
	if (HasElement(aValue))
	{
		return;
	}

	if (!myFirstNode)
	{
		TreeNode<T>* treeNode = new TreeNode<T>();
		treeNode->myValue = aValue;
		treeNode->myLeftNode = nullptr;
		treeNode->myRightNode = nullptr;

		myFirstNode = treeNode;
	}
	else
	{
		InsertElement(myFirstNode, aValue);
	}
}

template<class T>
void CommonUtilities::BSTSet<T>::InsertElement(TreeNode<T>* aNode, const T& aValue)
{

	if (!(aValue < aNode->myValue))
	{
		if (aNode->myLeftNode != NULL)
		{
			InsertElement(aNode->myLeftNode, aValue);
		}
		else
		{
			TreeNode<T>* treeNode = new TreeNode<T>();
			treeNode->myValue = aValue;
			treeNode->myLeftNode = nullptr;
			treeNode->myRightNode = nullptr;

			aNode->myLeftNode = treeNode;
		}
	}
	else if (!(aNode->myValue < aValue))
	{
		if (aNode->myRightNode != NULL)
		{
			InsertElement(aNode->myRightNode, aValue);
		}
		else
		{
			TreeNode<T>* treeNode = new CommonUtilities::TreeNode<T>();
			treeNode->myValue = aValue;
			treeNode->myLeftNode = nullptr;
			treeNode->myRightNode = nullptr;

			aNode->myRightNode = treeNode;
		}
	}
}

template<class T>
void CommonUtilities::BSTSet<T>::Remove(const T& aValue)
{
	if (!myFirstNode)
	{
		return;
	}

	if (!HasElement(aValue))
	{
		return;
	}

	RemoveElement(myFirstNode, myFirstNode, aValue);
}

template<class T>
void CommonUtilities::BSTSet<T>::RemoveElement(TreeNode<T>* aLastNode, TreeNode<T>* aNode, const T& aValue)
{
	if (!(aNode->myValue < aValue) && !(aValue < aNode->myValue))
	{
		if (myFirstNode == aNode)
		{

			if (!aNode->myLeftNode && aNode->myRightNode)
			{
				if (!aNode->myLeftNode)
				{
					aLastNode->myLeftNode = nullptr;
				}
				else
				{
					aLastNode->myLeftNode = aNode->myLeftNode;
				}
			}
			else if (!aNode->myRightNode && aNode->myLeftNode)
			{
				if (!aNode->myRightNode)
				{
					aLastNode->myRightNode = nullptr;
				}
				else
				{
					aLastNode->myRightNode = aNode->myRightNode;
				}
			}
			else if (aNode->myRightNode && aNode->myLeftNode)
			{
				TreeNode<T>* temp = aNode;
				TreeNode<T>* temp2 = nullptr;
				bool isDone = false;
				while (!isDone)
				{
					if (!temp->myRightNode)
					{
						isDone = true;
					}
					else
					{
						temp2 = temp;
						temp = temp->myRightNode;
					}
				}

				if (aLastNode->myLeftNode)
				{
					auto left = aNode->myLeftNode;
					temp->myLeftNode = left;

					if (temp2)
					{
						temp2->myLeftNode = nullptr;
					}
				}

				if (aLastNode->myRightNode)
				{
					auto left = aNode->myRightNode;
					temp->myRightNode = left;
					if (temp2)
					{
						temp2->myRightNode = nullptr;
					}
				}

				aLastNode->myLeftNode = temp;
			}



			if (aLastNode->myRightNode)
			{
				if (aLastNode->myRightNode == aNode)
				{
					aLastNode->myRightNode = nullptr;
				}
			}

			if (aLastNode->myLeftNode)
			{
				if (aLastNode->myLeftNode == aNode)
				{
					aLastNode->myLeftNode = nullptr;
				}
			}

			delete aNode;
		}
		else
		{
			if (aNode->myLeftNode)
			{
				RemoveElement(aNode, aNode->myLeftNode, aValue);
			}

			if (aNode->myRightNode)
			{
				RemoveElement(aNode, aNode->myRightNode, aValue);
			}
		}
	}
}

template<class T>
void CommonUtilities::BSTSet<T>::Print(const std::string& aPrefix, TreeNode<T>* aNode, bool isLeft)
{
	if (!myFirstNode)
	{
		std::cout << "Nothing in tree!" << std::endl;
		return;
	}

	if (!aNode)
	{
		aNode = myFirstNode;
	}

	std::cout << aPrefix;
	std::cout << (isLeft ? u8"├──" : u8"└──");
	std::cout << aNode->myValue << std::endl;

	if (aNode->myLeftNode)
	{
		Print(aPrefix + (isLeft ? u8"│   " : u8"    "), aNode->myLeftNode, true);
	}

	if (aNode->myRightNode)
	{
		Print(aPrefix + (isLeft ? u8"│   " : u8"    "), aNode->myRightNode, false);
	}
}

