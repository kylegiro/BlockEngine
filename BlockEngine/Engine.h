#pragma once

#include "Window.h"
#include "Shader.h"
#include "Quad.h"
#include "Chunk.h"
#include "Texture.h"
#include "Camera.h"
#include "ChunkManager.h"
#include "GUI.h"
#include "NoiseMap.h"
#include "Frustum.h"

enum class MouseMode
{
	Camera, 
	GUI
};

class Engine
{
public:
	Engine(SDL_GLContext glContext, Window& window);
	void init();
	void handleEvents();
	void update(double dt);
	void render();
	void quit();

	bool isDebugMode() const;
	bool* getDebugModeAdr();

	Frustum& getFrustum();

private:
	Window& window;
	Shader shader;

	Camera camera;
	Frustum frustum;
	GUI gui;
	SDL_GLContext glContext;

	Texture texture;
	ChunkManager chunkManager;

	MouseMode mouseMode = MouseMode::Camera;

	bool isQuit = false;
	bool debugMode = false;

	NoiseMap heightMap;

};

