#pragma once

#include "Window.h"
#include "Shader.h"
#include "Quad.h"
#include "Texture.h"
#include "Camera.h"

class Engine
{
public:
	Engine(Window& window, Shader& shader);
	void init();
	void handleEvents();
	void update();
	void render();
	void quit();

private:
	Window& window;
	Shader& shader;

	Camera camera;

	Texture texture;
	Quad quad;

	bool isQuit = false;

};

