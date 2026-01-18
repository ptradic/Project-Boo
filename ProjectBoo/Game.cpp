#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "TransformComponent.h"

using namespace std;

Map* NewMap;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1040 * 2,1120 * 2 };

bool Game::isRunning = false;

auto& player(manager.addEntity());

Game::Game() {
}
Game::~Game() {
}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	
	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	
	NewMap = new Map("assets/TileSetNew.png", 3, 32);
	
	NewMap->LoadMap("assets/Maze.map", 30, 30);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("assets/player_anims.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents() {

	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update() {
	
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;


	manager.refersh();
	manager.upDate();

	if (player.getComponent<TransformComponent>().position.x >= 2800) {

		isRunning = false;
	}
	

	for (auto& c : colliders) {

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {

			player.getComponent<TransformComponent>().position = playerPos;

		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - (400 - 60);
	camera.y = player.getComponent<TransformComponent>().position.y - (320 - 60);

	if (camera.x < 0) 
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
	
	Timer();
}

void Game::render() {

	SDL_RenderClear(renderer);

	for (auto& t : tiles) {
		t->draw();
	}

	/*for (auto& c : colliders) {
		c->draw();
	}
	*/
	for (auto& p : players) {
		p->draw();
	}
	
	SDL_RenderPresent(renderer);
}

void Game::clean() {

	Scoreboard();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


void Game::Scoreboard() {

	string playername;

	cout << "Please enter your name." << endl;
	cin >> playername;
	cout << endl;

	ofstream file;

	file.open("Scoreboard.txt", ios::app);

	file << playername << ": " << sec << endl;

	file.close();
}

void Game::Timer() {

	Uint32 ms = SDL_GetTicks();

	sec = ms / 1000;
	//system("cls");
	cout << sec << endl;

}