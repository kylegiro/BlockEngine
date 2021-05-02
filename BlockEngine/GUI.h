#pragma once

#include <SDL.h>
#include <string>

#include "Camera.h"
#include "ChunkManager.h"

class Engine;

class GUI
{
public:
	GUI(SDL_Window* window, SDL_GLContext glContext, ChunkManager& chunkManager, NoiseMap& heightMap, Engine& engine);
	~GUI();
	
	void render(Camera& camera);
	void updateCameraPositionLabel(Camera& camera);

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	ChunkManager& chunkManager;
	NoiseMap& heightMap;
	Engine& engine;

	std::string cameraPos = "Position: NULL";
	std::string chunkPos = "Chunk: NULL";
	bool debugCheckBox = false;

};

