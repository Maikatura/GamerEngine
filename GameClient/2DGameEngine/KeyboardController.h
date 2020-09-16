#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {
public:
	TransformComponent *transform;

	void Init() override {
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Update() 
	{
		if (Game::event.type == SDL_KEYDOWN) 
		{
			switch (Game::event.key.keysym.sym) 
			{
				case SDLK_d:
					transform->velocity.x = 1;
					break;
				case SDLK_a:
					transform->velocity.x = -1;
					break;
				
				case SDLK_ESCAPE:
					SDL_QUIT;
					break;
				default:
					break;
			}
		}

		if (Game::event.type == SDL_KEYUP) 
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_d:
				transform->velocity.x = 0;
				break;

			case SDLK_a:
				transform->velocity.x = 0;
				break;
			default:
				break;
			}
		}
	}
};
