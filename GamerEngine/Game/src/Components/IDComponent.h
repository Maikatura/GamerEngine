#pragma once
#include "Component.h"
#include "Scene/UUID.h"

class IDComponent : public Component
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
