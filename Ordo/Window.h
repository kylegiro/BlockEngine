#pragma once

#include "SDL.h"

class Window
{
public:
	Window();
	~Window();
	Window& create();

private:
	SDL_Window* sdlWindow;

};

