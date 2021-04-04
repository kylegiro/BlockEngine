#include "Engine.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Engine::Engine(Window& window, Shader& shader) 
    : window(window), shader(shader),
    texture(),
    quad(1.0f, 1.0f, 1.0f, 1.0f, texture),
    camera(glm::vec3(0.0f, 0.0f, 0.0f))
{       
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);
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

        if (event.type == SDL_KEYDOWN)
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

void Engine::update()
{
}

void Engine::render()
{
    glClearColor(0.207f, 0.337f, 0.537f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glm::mat4 view = camera.getView();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    quad.render(shader);

    window.render();
}

void Engine::quit()
{
    isQuit = true;
}
