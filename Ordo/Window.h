#pragma once

#include "SDL.h"
#include "SDLManager.h"
#include <string>

class Window
{
public:
	Window(SDLManager& sdlManager, std::string title, int width, int height);
	~Window();
	Window& init();
	void render();

private:
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	SDLManager& sdlManager;
	int width, height;
	std::string title;

};

