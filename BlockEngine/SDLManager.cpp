#include "SDL.h"
#include "SDLManager.h"
#include "Window.h"

#include <stdio.h>
#include <glad/glad.h>


SDLManager::SDLManager(Window& window) : window(window)
{
    init();
}

void SDLManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        die();
    }

    //SDL_GL_LoadLibrary(NULL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    glContext = SDL_GL_CreateContext(window.sdlWindow);
    SDL_GL_MakeCurrent(window.sdlWindow, glContext);

    // GLAD Init
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        die();
    }
}

void SDLManager::quit()
{
    SDL_Quit();
}

void SDLManager::die()
{
    printf("FATAL: SDL fatal error. SDL_Error: %s\n", SDL_GetError());
    quit();
}

SDL_GLContext SDLManager::getContext()
{
    return glContext;
}
