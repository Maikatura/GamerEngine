#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "SDL_net.h"
#include "Client.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Client client;

Manager manager;
auto& splashScreen(manager.AddEntity());
auto& player(manager.AddEntity());

auto& animatedTexture(manager.AddEntity());
auto& animatedTexture2(manager.AddEntity());

std::vector<ColliderComponent*> Game::colliders;

Uint8* SDL_GetKeyState(int* numkeys);

Game::Game() {

}

Game::~Game() {

}

bool KEYS[322];

void Game::Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) 
{
	client.Init();
	
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
	

	// Debugging Stuff
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 64, 64, 2);
	player.AddComponent<KeyboardController>();
	player.AddComponent<SpriteComponent>("assets/Player/Player_Idle.png", 5, 150);
	player.AddComponent<ColliderComponent>("player");
	player.AddComponent<Rigidbody2D>();

	animatedTexture2.AddComponent<TransformComponent>(100.0f, 400.0f, 400, 200, 1);
	animatedTexture2.AddComponent<SpriteComponent>("assets/Tiles/grass.png");
	animatedTexture2.AddComponent<ColliderComponent>("wall");

	animatedTexture.AddComponent<TransformComponent>(0.0f, 300.0f, 300, 200, 1);
	animatedTexture.AddComponent<SpriteComponent>("assets/Tiles/grass.png");
	animatedTexture.AddComponent<ColliderComponent>("wall");
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

	for (auto c : colliders) 
	{
		

		//if (Collision::AABB(player.GetComponent<ColliderComponent>(), *c)) 
		//{
		//	player.GetComponent<TransformComponent>().velocity.y = 0;
		//}
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
