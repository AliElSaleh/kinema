#pragma once

#include <iostream>

#include "SDL2/SDL.h"

class Engine
{
public:
	Engine();
	~Engine();

private:
	SDL_Window* Window;
	SDL_GLContext Context;

	void Update();
	void Render();
};