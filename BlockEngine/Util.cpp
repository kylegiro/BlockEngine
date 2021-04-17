#include "Util.h"
#include "Chunk.h"

glm::ivec3 worldToChunk(glm::vec3 world)
{    
    int chunkX = (int)(world.x / CHUNK_SIZE);
    int chunkY = (int)(world.y / CHUNK_SIZE);
    int chunkZ = (int)(world.z / CHUNK_SIZE);

    if (world.x < 0.0f)    
        chunkX -= 1;
    if (world.y < 0.0f)
        chunkY -= 1;
    if (world.z < 0.0f)
        chunkZ -= 1;

    return glm::ivec3(chunkX, chunkY, chunkZ);
}
