#include "Util.h"
#include "Chunk.h"

glm::ivec3 worldToChunk(glm::vec3 world)
{    
    return glm::ivec3(world.x / CHUNK_SIZE, world.y / CHUNK_SIZE, world.z / CHUNK_SIZE);
}
