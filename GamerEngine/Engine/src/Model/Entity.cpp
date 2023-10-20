#include "GraphicsEngine.pch.h"
#include <Model/Entity.h>

#include <Components/Components.hpp>
#include <iostream>

#include "Components/NativeScriptComponent.h"
#include "Components/RandomMoverComponent.h"


template <typename T>
T* Entity::GetScriptComponent()
{
	if(auto component = dynamic_cast<const T*>(GetComponent<NativeScriptComponent>().Instance))
	{
		return component;
	}

	return nullptr;
}
