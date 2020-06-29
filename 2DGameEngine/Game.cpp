#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"

GameObject* player;
Map* map;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {

}

Game::~Game() {

}

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

	player = new GameObject("assets/Player/Maika_Idle.png", 10, 10);
	map = new Map();
	
}

void Game::HandleEvents() {
	SDL_Event event;
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
	player->Update();
}

void Game::Render() {
	SDL_RenderClear(renderer);
	// Add stuff to render;
	map->DrawMap();
	player->Render();


	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned!" << std::endl;
}