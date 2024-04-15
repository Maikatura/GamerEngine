#pragma once
#include "Scene/UUID.h"

class IDComponent 
{
public:
	GamerEngine::UUID ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
	IDComponent(GamerEngine::UUID aUUID)
	{
		ID = aUUID;
	}
};
