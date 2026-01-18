#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <Windows.h>


using namespace std;

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();
	void Scoreboard();
	void Timer();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;

	enum groupLabels : size_t {

		groupMap,
		groupPlayers,
		groupColliders
	};

private:
	
	int cnt = 0;
	int sec = 0;
	SDL_Window* window;
};