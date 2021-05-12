#pragma once

#include <glm/glm.hpp>

glm::ivec3 blockAt(glm::vec3 world);
glm::ivec3 chunkAtBlock(glm::ivec3 block);
glm::ivec3 toChunkInternal(glm::ivec3 block);