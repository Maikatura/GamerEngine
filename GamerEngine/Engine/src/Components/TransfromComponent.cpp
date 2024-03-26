#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"

#include "Model/Entity.h"

std::vector<Entity*> TransformComponent::GetChildren() const
{
	return myChildren;
}

bool TransformComponent::HasParent()
{
	if (!myParent)
	{
		return false;
	}

	return (myParent->GetHandle() != entt::null);
}
