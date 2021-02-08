#include "Engine.h"

Engine::Engine(Window& window) : window(window)
{
    init();
}

void Engine::init()
{
    while(!isQuit)
    {
        handleEvents();
        update();
        render();
    }
}

void Engine::handleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {            
            isQuit = true;
        }
    }
}

void Engine::update()
{
}

void Engine::render()
{
    window.render();
}

void Engine::quit()
{
    isQuit = true;
}
