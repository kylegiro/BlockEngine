#pragma once

#include "Window.h"
#include "Shader.h"
#include "Quad.h"
#include "Chunk.h"
#include "Texture.h"
#include "Camera.h"
#include "ChunkManager.h"

class Engine
{
public:
	Engine(Window& window, Shader& shader);
	void init();
	void handleEvents();
	void update(double dt);
	void render();
	void quit();

private:
	Window& window;
	Shader& shader;

	Camera camera;

	Texture texture;	
	ChunkManager chunkManager;

	bool isQuit = false;

};

