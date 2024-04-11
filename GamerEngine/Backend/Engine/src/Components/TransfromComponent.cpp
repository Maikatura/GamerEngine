#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"

#include "Core/Model/Entity.h"

std::vector<GamerEngine::Entity*> TransformComponent::GetChildren() const
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
