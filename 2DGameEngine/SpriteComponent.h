#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

	

public:

	int topSide;

	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		SetTex(path);
	}

	SpriteComponent(const char* path, int nFrames, int mSpeed)
	{
		animated = true;
		frames = nFrames;
		speed = mSpeed;
		SetTex(path);
	}


	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}


	void SetTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void Init() override 
	{
		transform = &entity->GetComponent<TransformComponent>();

		
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

	}

	void Update() override
	{
		if (animated) 
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}
};

