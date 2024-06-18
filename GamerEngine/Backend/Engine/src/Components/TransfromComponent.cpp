#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"





std::vector<entt::entity> GamerEngine::TransformComponent::GetChildren() const
{
	return myChildren;
}

bool GamerEngine::TransformComponent::HasParent() const
{
	return (myParent != entt::null);
}
