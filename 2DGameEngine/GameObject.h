#pragma once
#include "Game.h"

class GameObject
{
public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:

	int xPos;
	int yPos;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};

