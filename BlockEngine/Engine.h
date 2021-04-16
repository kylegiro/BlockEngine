#pragma once

#include "Window.h"
#include "Shader.h"
#include "Quad.h"
#include "Chunk.h"
#include "Texture.h"
#include "Camera.h"
#include "ChunkManager.h"
#include "GUI.h"

enum class MouseMode
{
	Camera, 
	GUI
};

class Engine
{
public:
	Engine(SDL_GLContext glContext, Window& window, Shader& shader);
	void init();
	void handleEvents();
	void update(double dt);
	void render();
	void quit();

private:
	Window& window;
	Shader& shader;

	Camera camera;
	GUI gui;
	SDL_GLContext glContext;

	Texture texture;	
	ChunkManager chunkManager;

	MouseMode mouseMode = MouseMode::Camera;

	bool isQuit = false;

};

