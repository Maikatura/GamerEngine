#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"





std::vector<GamerEngine::Entity> TransformComponent::GetChildren() const
{
	return myChildren;
}

bool TransformComponent::HasParent() const
{
	if (!myParent)
	{
		return false;
	}

	return (myParent.GetHandle() != entt::null);
}
