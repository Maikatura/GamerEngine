#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) 
{
	objTexture = TextureManager::LoadTexture(textureSheet);

	xPos = x;
	yPos = y;
}

void GameObject::Update()
{
	xPos = 0;
	yPos = 0;

	srcRect.h = 64;
	srcRect.w = 64;

	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}


void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}