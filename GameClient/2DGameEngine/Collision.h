#pragma once
#include "SDL.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Collision 
{
public: 
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB, const std::string& tag);
};
