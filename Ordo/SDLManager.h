#pragma once
#include <SDL.h>

class Window;

class SDLManager
{
public:
	SDLManager(Window& window);
	void init();
	void quit();
	void die();

private:
	Window& window;
	SDL_GLContext glContext;

};

