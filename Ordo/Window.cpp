#include "Window.h"
#include <stdio.h>

Window::Window(SDLManager& sdlManager) : sdlManager(sdlManager)
{
    init();
}

Window& Window::init()
{
    sdlWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    if (sdlWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    return *this;
}

void Window::render()
{
    SDL_GL_SwapWindow(sdlWindow);
}


Window::~Window()
{
	SDL_DestroyWindow(sdlWindow);
}
