#include "Camera.h"
#include <iostream>

const float SPEED = 10.0F;
const float SPRINT_SPEED = 25.0F;

Camera::Camera(glm::vec3 position) :
    position(position),
    front(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    right(1.0f, 0.0f, 0.0f),
    worldUp(up),
    yaw(-90.0f),
    pitch(0.0f),
    speed(SPEED),
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
        setMoveState(MovementFlag::FORWARD, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_A)
        setMoveState(MovementFlag::LEFT, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_S)
        setMoveState(MovementFlag::BACKWARD, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_D)
        setMoveState(MovementFlag::RIGHT, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_SPACE)
        setMoveState(MovementFlag::UP, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_LCTRL)
        setMoveState(MovementFlag::DOWN, e.state == SDL_PRESSED ? true : false);
    if (key == SDL_SCANCODE_LSHIFT)
        setMoveState(MovementFlag::SPRINT, e.state == SDL_PRESSED ? true : false);
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

void Camera::update(double dt)
{
    if (getMoveState(MovementFlag::SPRINT) == true)
        speed = SPRINT_SPEED;
    else
        speed = SPEED;

    if (getMoveState(MovementFlag::FORWARD ))
        position += front * speed * (float)dt;
    if (getMoveState(MovementFlag::LEFT))
        position -= right * speed * (float)dt;
    if (getMoveState(MovementFlag::BACKWARD))
        position -= front * speed * (float)dt;
    if (getMoveState(MovementFlag::RIGHT))
        position += right * speed * (float)dt;
    if (getMoveState(MovementFlag::UP))
        position += worldUp * speed * (float)dt;
    if (getMoveState(MovementFlag::DOWN))
        position -= worldUp * speed * (float)dt;
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

void Camera::setMoveState(MovementFlag flag, bool on)
{
    if(on)
        moveState |= (int)flag;
    else
        moveState &= ~(int)flag;
}

bool Camera::getMoveState(MovementFlag flag)
{
    return (moveState & (int)flag) == (int)flag;
}
