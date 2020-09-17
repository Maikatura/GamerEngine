#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "Collision.h";

class ColliderComponent;

class Rigidbody2D : public Component {

public: 

	TransformComponent *transform;
	ColliderComponent* collider;

	void Init() override 
	{
		transform = &entity->GetComponent<TransformComponent>();
		collider = &entity->GetComponent<ColliderComponent>();
	}

	void Update() override 
	{
		transform->velocity.y = 1;

		for (auto c : Game::colliders) 
		{
			if (Collision::AABB(*collider, *c))
			{
				transform->velocity.y = 0;
			}
		}
	}


private:

};
