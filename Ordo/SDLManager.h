#pragma once
#include <SDL.h>

class Window;

class SDLManager
{
public:
	SDLManager();
	void init();
	void createContext(Window& window); // create GL context for the SDL window
	void quit();
	void die();

private:
	SDL_GLContext glContext;

};

