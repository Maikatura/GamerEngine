#pragma once
#include "Components/ScriptableEntity.h"


class Animator : public ScriptableEntity
{
public:

	void OnCreate() override
	{
		myModelComponent = GetComponent<ModelComponent>();
	}
	

	void OnUpdate() override
	{
		std::cout << "LOL" << std::endl;
	}

private:
	ModelComponent myModelComponent;

};
