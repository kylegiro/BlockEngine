#include "Window.h"
#include <glad/glad.h>
#include <stdio.h>
#include <string>

Window::Window(std::string title, int width, int height)
    : title(title),
    width(width),
    height(height)
{
    init();
}

Window& Window::init()
{
    sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if (sdlWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
       
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
