#include "SDL.h"
#include "SDLManager.h"

#include <stdio.h>

SDLManager::SDLManager()
{
    init();
}

void SDLManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        die();
    }

    SDL_GL_LoadLibrary(NULL);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void SDLManager::quit()
{
    SDL_Quit();
}

void SDLManager::die()
{
    printf("WARNING: SDL fatal error. SDL_Error: %s\n", SDL_GetError());
    quit();
}
