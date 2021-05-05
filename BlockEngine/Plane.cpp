#include "Plane.h"

#include <sstream>

Plane::Plane() : normal(), d(0)
{
}

Plane::Plane(float a, float b, float c, float d)
{
	normal = glm::vec3(a, b, c);

	float length = glm::length(normal);

	normal = glm::vec3(a / length, b / length, c / length);

	this->d = d / length;
}

Plane::Plane(glm::vec3 point, glm::vec3 normal)
{
	this->normal = normal;

	this->normal = glm::normalize(this->normal);
	d = -glm::dot(normal, point);
}

float Plane::distance(glm::vec3 point)
{
	return normal.x * point.x + normal.y * point.y + normal.z * point.z + d;
}

std::string Plane::toString()
{
	std::ostringstream ss;
	ss << "(" << normal.x << "," << normal.y << "," << normal.z << "," << d << ")";
	return ss.str();
}
