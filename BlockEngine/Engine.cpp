#include "Engine.h"
#include "Texture.h"
#include "Raycast.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <iostream>

using namespace std::chrono;

Engine::Engine(SDL_GLContext glContext, Window& window)
    : glContext(glContext),   
    window(window),
    shader("shader.vert", "shader.frag"),
    atlas(256, 256, "textures/atlas.png"),
    frustum(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f)),
    camera(glm::vec3(0.0f, 0.0f, 0.0f)),
    heightMap(),
    chunkManager(shader, atlas, camera, frustum, heightMap),
    gui(window.getSDLWindow(), glContext, chunkManager, heightMap, *this)
{
    glEnable(GL_DEPTH_TEST);

    SDL_SetRelativeMouseMode(SDL_TRUE);

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
       
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_C)
            {
                if (mouseMode == MouseMode::Camera)
                {
                    mouseMode = MouseMode::GUI;
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    window.centerMouse();
                }
                else
                {
                    mouseMode = MouseMode::Camera;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }

            }
        }       

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            camera.handleInput(event.key);           
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            if(mouseMode == MouseMode::Camera)
                camera.handleMouse(event.motion);
        }

        if (event.type = SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Raycast raycast(camera.getPosition(), camera.getFront(), 10.0f);
                std::vector<glm::ivec3> blocks = raycast.getBlocks();
                for (std::vector<glm::ivec3>::iterator it = blocks.begin(); it != blocks.end(); it++)
                {
                    glm::ivec3 coord = *it;
                    std::cout << "(" << coord.x << "," << coord.y << "," << coord.z << ",)" << std::endl;
                }
            }
        }
    }
}

void Engine::update(double dt)
{
    camera.update(dt);
    chunkManager.update(dt);
    gui.updateCameraPositionLabel(camera);
}

void Engine::render()
{
    glClearColor(0.207f, 0.337f, 0.537f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 view = camera.getView();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

    frustum = Frustum(projection * view);

    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    chunkManager.render();
    if (isDebugMode())
        chunkManager.renderDebug();
    gui.render(camera);

    window.render();
}

void Engine::quit()
{
    isQuit = true;
}

bool Engine::isDebugMode() const
{
    return debugMode;
}

bool* Engine::getDebugModeAdr()
{
    return &debugMode;
}

Frustum& Engine::getFrustum()
{
    return frustum;
}
