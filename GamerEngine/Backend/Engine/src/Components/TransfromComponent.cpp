#include "GraphicsEngine.pch.h"
#include "TransfromComponent.h"





std::vector<uint64_t> GamerEngine::TransformComponent::GetChildren() const
{
	return myChildren;
}

bool GamerEngine::TransformComponent::HasParent() const
{
	return (myParent != 0);
}
