#include "Raycast.h"

#include "Util.h"

Raycast::Raycast(glm::vec3 origin, glm::vec3 direction, float length)
	: origin(origin), direction(direction), length(length)
{
	glm::ivec3 block = blockAt(origin);
	int x = block.x;
	int y = block.y;
	int z = block.z;

	int stepX = direction.x < 0 ? -1 : 1;
	int stepY = direction.y < 0 ? -1 : 1;
	int stepZ = direction.z < 0 ? -1 : 1;

	float tMaxX = tmax(origin.x, direction.x);
	float tMaxY = tmax(origin.y, direction.y);
	float tMaxZ = tmax(origin.z, direction.z);

	float tDeltaX = stepX / direction.x;
	float tDeltaY = stepY / direction.y;
	float tDeltaZ = stepZ / direction.z;

	while (true)
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				if (tMaxX > length) break;
				x += stepX;
				tMaxX += tDeltaX;

				blocks.push_back(glm::ivec3(x, y, z));
			}
			else
			{
				if (tMaxZ > length) break;
				z += stepZ;
				tMaxZ += tDeltaZ;

				blocks.push_back(glm::ivec3(x, y, z));
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				if (tMaxY > length) break;
				y += stepY;
				tMaxY += tDeltaY;

				blocks.push_back(glm::ivec3(x, y, z));
			}
			else
			{
				if (tMaxZ > length) break;
				z += stepZ;
				tMaxZ += tDeltaZ;

				blocks.push_back(glm::ivec3(x, y, z));
			}
		}
	}
}

std::vector<glm::ivec3>& Raycast::getBlocks()
{
	return blocks;
}

float Raycast::tmax(float o, float d)
{
	if (d < 0 && round(o) == o)
		return 0;

	return (d > 0 ? ceil(o) - o : o - floor(o)) / abs(d);
}
