#pragma once
#include "GraphicsEngine.h"
#include "Model/Entity.h"


struct ChildComponent
{
	Entity myParent;
	std::vector<Entity> myChildren{};

	void SetChild(Entity aChild)
	{
		myChildren.push_back(aChild);
	}

	void RemoveChild(Entity aChild)
	{
		for(size_t i = 0; i < myChildren.size(); i++)
		{
			if(myChildren[i] == aChild)
			{
				myChildren.erase(myChildren.begin() + i);
				i--;
			}
		}
	}

	void SetParent(Entity aParent)
	{
		myParent = aParent;
	}

	void ClearParent()
	{
		myParent = { entt::null, nullptr };
	}

	const Entity GetParent() const
	{
		return myParent;
	}

	std::vector<Entity> GetChildren() const
	{
		return myChildren;
	}

	bool HasParent()
	{
		return (myParent.GetHandle() != entt::null);
	}
};
