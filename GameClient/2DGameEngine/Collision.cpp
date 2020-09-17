#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) 
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		) 
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& a, const ColliderComponent& b)
{
	if (AABB(a.collider, b.collider))
	{
		if (b.tag != "player") {
			std::cout << a.tag << " Hit: " << b.tag << std::endl;
			//std::cout << colA.tag << " hit: " << colB.tag << std::endl;
			return true;
		}
		else 
		{
			return false;
		}	
	}
	else
	{
		return false;
	}
}


// Look for tag in colliders past in then check if it collides and has the tag
bool Collision::AABB(const ColliderComponent& a, const ColliderComponent& b, const std::string& tag)
{
	if (a.tag == tag || b.tag == tag) 
	{
		if (AABB(a.collider, b.collider))
		{
			if (b.tag != "player") 
			{
				std::cout << a.tag << " Hit: " << b.tag << std::endl;
				return true;
			}
			else 
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	
}