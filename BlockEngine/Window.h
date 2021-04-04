#pragma once

#include "SDL.h"
#include "SDLManager.h"
#include <string>

class Window
{
public:
	Window(std::string title, int width, int height);
	~Window();
	Window& init();
	void render();

private:
	friend class SDLManager; // allow access to SDL_Window raw pointer

	SDL_Window* sdlWindow;
	int width, height;
	std::string title;

};

