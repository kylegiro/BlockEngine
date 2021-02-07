#pragma once

#include "Window.h"

class Engine
{
public:
	Engine(Window& window);
	void init();
	void handleEvents();
	void update();
	void render();
	void quit();

private:
	Window& window;
	bool isQuit = false;

};

