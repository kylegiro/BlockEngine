#pragma once

#include <SDL.h>
#include <string>

#include "Camera.h"
#include "ChunkManager.h"

class GUI
{
public:
	GUI(SDL_Window* window, SDL_GLContext glContext, ChunkManager& chunkManager);
	~GUI();
	
	void render(Camera& camera);
	void updateCameraPositionLabel(Camera& camera);

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	ChunkManager& chunkManager;

	std::string cameraPos = "Position: NULL";
	std::string chunkPos = "Chunk: NULL";

};

