#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"


class Rigidbody2D : public Component {

public: 

	TransformComponent *transform;

	void Init() override 
	{
		transform = &entity->GetComponent<TransformComponent>();
	}


	void Update() override 
	{
		transform->velocity.y = 1;



	}
	



private:

};
