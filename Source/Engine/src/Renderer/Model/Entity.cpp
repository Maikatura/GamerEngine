#include "GraphicsEngine.pch.h"
#include <Renderer/Model/Entity.h>

#include <Components/Components.hpp>
#include <iostream>

template <typename T>
T* Entity::GetScriptComponent()
{
	if(dynamic_cast<T*>(GetComponent<NativeScriptComponent>().Instance) != nullptr)
	{
		return (T*)GetComponent<NativeScriptComponent>().Instance;
	}

	return nullptr;
}
