#pragma once
#include "Scene/UUID.h"

class IDComponent 
{
public:
	UUID2 ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
	IDComponent(UUID2 aUUID)
	{
		ID = aUUID;
	}
};
