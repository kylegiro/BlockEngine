#pragma once

#include <SDL.h>

class GUI
{
public:
	GUI(SDL_Window* window, SDL_GLContext glContext);
	~GUI();
	
	void render();

private:
	SDL_Window* window;
	SDL_GLContext glContext;

};

