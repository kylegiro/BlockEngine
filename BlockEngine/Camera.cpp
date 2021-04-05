#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position) :
    position(position),
    front(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    right(1.0f, 0.0f, 0.0f),
    worldUp(up),
    yaw(-90.0f),
    pitch(0.0f),
    speed(0.5f),
    sensitivity(0.25f)
{
    updateVectors();
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::handleInput(SDL_KeyboardEvent e)
{
    SDL_Scancode key = e.keysym.scancode;

    if (key == SDL_SCANCODE_W)
        position += front * speed;
    if (key == SDL_SCANCODE_A)
        position -= right * speed;
    if (key == SDL_SCANCODE_S)
        position -= front * speed;
    if (key == SDL_SCANCODE_D)
        position += right * speed;
    if (key == SDL_SCANCODE_SPACE)
        position += worldUp * speed;
    if (key == SDL_SCANCODE_LCTRL)
        position -= worldUp * speed;
}

void Camera::handleMouse(SDL_MouseMotionEvent e)
{
    float x = e.xrel;
    float y = e.yrel;

    yaw += x * sensitivity;
    pitch -= y * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;    

    updateVectors();
}

void Camera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));
}
