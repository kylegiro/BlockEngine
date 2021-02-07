#include "Window.h"
#include <stdio.h>

Window::Window()
{
    create();
}

Window& Window::create()
{
    sdlWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (sdlWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    return *this;
}


Window::~Window()
{
	SDL_DestroyWindow(sdlWindow);
}
