#pragma once

#include <glm/glm.hpp>
#include <vector>

class Raycast
{
public:
	Raycast(glm::vec3 origin, glm::vec3 direction, float length);
	std::vector<glm::ivec3>& getBlocks();

private:
	glm::vec3 origin;
	glm::vec3 direction;
	float length;
	std::vector<glm::ivec3> blocks;

	float tmax(float o, float d);
};

