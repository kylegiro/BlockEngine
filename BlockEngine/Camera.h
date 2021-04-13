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
	void update(double dt);
	glm::vec3 getPosition();

private:
	glm::vec3 position, front, up, right, worldUp;
	float yaw, pitch;
	float speed, sensitivity;	

	void updateVectors();

	enum class MovementFlag
	{
		FORWARD = 1 << 0,
		RIGHT = 1 << 1,
		BACKWARD = 1 << 2,
		LEFT = 1 << 3,
		UP = 1 << 4,
		DOWN = 1 << 5,
		SPRINT =  1 << 6
	};

	uint8_t moveState = 0;

	void setMoveState(MovementFlag flag, bool on);
	bool getMoveState(MovementFlag flag);
};

