#pragma once

#include <glm/glm.hpp>
#include <string>

class Plane
{
public:
	Plane();
	Plane(float a, float b, float c, float d);
	Plane(glm::vec3 point, glm::vec3 normal);
	float distance(glm::vec3 point);
	std::string toString();

private:
	glm::vec3 normal;
	float d; // dot product of normal and point
};

