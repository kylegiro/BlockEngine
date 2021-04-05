#pragma once

#include "SDL.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position);
	glm::mat4 getView();
	void handleInput(SDL_KeyboardEvent e);
	void handleMouse(SDL_MouseMotionEvent e);	

private:
	glm::vec3 position, front, up, right, worldUp;
	float yaw, pitch;
	float speed, sensitivity;

	void updateVectors();
};

