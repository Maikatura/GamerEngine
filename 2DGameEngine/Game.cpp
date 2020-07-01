#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.AddEntity());
auto& wall(manager.AddEntity());

Uint8* SDL_GetKeyState(int* numkeys);

Game::Game() {

}

Game::~Game() {

}

bool KEYS[322];

void Game::Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) {

	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow("Title", xPos, yPos, width, height, flags);
		if (window) 
		{
			std::cout << "Window Created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer Created!" << std::endl;
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	  // 322 is the number of SDLK_DOWN events

	for (int i = 0; i < 322; i++) { // init them all to false
		KEYS[i] = false;
	}

	
	
	map = new Map();
	
	player.AddComponent<TransformComponent>(300.0f, 300.0f, 64, 64, 2);
	player.AddComponent<KeyboardController>();
	player.AddComponent<SpriteComponent>("assets/Player/Maika_Idle.png");
	player.AddComponent<ColliderComponent>("player");

	wall.AddComponent<TransformComponent>(400.0f, 300.0f, 20, 300, 1);
	wall.AddComponent<SpriteComponent>("assets/Tiles/grass.png");
	wall.AddComponent<ColliderComponent>("wall");
}

void Game::HandleEvents() {

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
	}
		
}

void Game::Update() 
{
	

	manager.Refresh();
	manager.Update();
	
	if (Collision::AABB(player.GetComponent<ColliderComponent>().collider, wall.GetComponent<ColliderComponent>().collider)) 
	{
		std::cout << "Wall Hit!" << std::endl;
	}
	
}

void Game::Render() {
	SDL_RenderClear(renderer);
	// Add stuff to render;
	map->DrawMap();
	manager.Draw();

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned!" << std::endl;
}
