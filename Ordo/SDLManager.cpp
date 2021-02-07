#include "SDL.h"
#include "SDLManager.h"

#include <stdio.h>

SDLManager::SDLManager()
{
}

void SDLManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        initialized = false;
    }
}

void SDLManager::quit()
{
    SDL_Quit();
}
