#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"





std::vector<GamerEngine::Entity> GamerEngine::TransformComponent::GetChildren() const
{
	return myChildren;
}

bool GamerEngine::TransformComponent::HasParent() const
{
	if (!myParent)
	{
		return false;
	}

	return (myParent.GetHandle() != entt::null);
}
