#include "Engine.h"
#include <glad/glad.h>

Engine::Engine(Window& window, Shader& shader) : window(window), shader(shader)
{
    quad = Quad();
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    quad.render();

    window.render();
}

void Engine::quit()
{
    isQuit = true;
}
