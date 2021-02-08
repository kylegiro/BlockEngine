#pragma once

#include "SDL.h"
#include "SDLManager.h"

class Window
{
public:
	Window(SDLManager& sdlManager);
	~Window();
	Window& init();
	void render();

private:
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	SDLManager& sdlManager;

};

