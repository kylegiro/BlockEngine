#pragma once

#include <SDL.h>
#include <string>

#include "Camera.h"

class GUI
{
public:
	GUI(SDL_Window* window, SDL_GLContext glContext);
	~GUI();
	
	void render();
	void updateCameraPositionLabel(Camera& camera);

private:
	SDL_Window* window;
	SDL_GLContext glContext;

	std::string cameraPos = "Position: NULL";
	std::string chunkPos = "Chunk: NULL";

};

