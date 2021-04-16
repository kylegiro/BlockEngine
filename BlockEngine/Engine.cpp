#include "Engine.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <iostream>

using namespace std::chrono;

Engine::Engine(SDL_GLContext glContext, Window& window, Shader& shader)
    : glContext(glContext),
    window(window), shader(shader),
    texture(),
    camera(glm::vec3(0.0f, 0.0f, 0.0f)),
    chunkManager(shader, texture, camera),
    gui(window.getSDLWindow(), glContext)
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);

    init();
}

void Engine::init()
{
    double t = 0.0;
    const double dt = 0.01666 * 1000; // 60 ticks per second

    long long currentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double accumulator = 0.0;

    while(!isQuit)
    {
        handleEvents();

        long long newTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        long long frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= dt)
        {
            update(accumulator / 1000);
            t += dt;
            accumulator -= dt;            
        }
        
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

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            camera.handleInput(event.key);

        }

        if (event.type == SDL_MOUSEMOTION)
        {
            camera.handleMouse(event.motion);
            window.centerMouse();
        }
    }
}

void Engine::update(double dt)
{
    camera.update(dt);
    chunkManager.update(dt);
}

void Engine::render()
{
    glClearColor(0.207f, 0.337f, 0.537f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 view = camera.getView();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    gui.render();

    chunkManager.render();

    window.render();
}

void Engine::quit()
{
    isQuit = true;
}
