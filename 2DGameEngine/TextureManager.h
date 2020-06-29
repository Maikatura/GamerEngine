#pragma once

#include "Game.h"


class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect, SDL_Rect dest);
};
