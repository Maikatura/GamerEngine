#ifndef Game_hpp
#define Game_hpp

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <iostream>
#include <vector>

class ColliderComponent;

class Game 
{
	public:
		Game();
		~Game();

		void Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

		void HandleEvents();
		void Update(); // Update the game
		bool Running() { return isRunning; }
		void Render(); // Render Items on screen
		void Clean();

		static SDL_Renderer* renderer;
		static SDL_Event event;

		static std::vector<ColliderComponent*> colliders;

	private:
		int cnt;
		bool isRunning;
		SDL_Window* window;
		
};

#endif // !Game_hpp


