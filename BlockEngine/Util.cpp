#include "Util.h"
#include "Chunk.h"

#include <iostream>

glm::ivec3 blockAt(glm::vec3 world)
{
    int x = world.x;
    int y = world.y;
    int z = world.z;

    if (world.x < 0.0f)
        x -= 1;
    if (world.y < 0.0f)
        y -= 1;
    if (world.z < 0.0f)
        z -= 1;

    return glm::ivec3(x, y, z);
}

glm::ivec3 chunkAtBlock(glm::ivec3 block)
{    
    if (block.x < -1.0f)
        block.x += 1;
    if (block.y < -1.0f)
        block.y += 1;
    if (block.z < -1.0f)
        block.z += 1;

    int chunkX = (int)(block.x / CHUNK_SIZE);
    int chunkY = (int)(block.y / CHUNK_SIZE);
    int chunkZ = (int)(block.z / CHUNK_SIZE);

    if (block.x < 0.0f)
        chunkX -= 1;
    if (block.y < 0.0f)
        chunkY -= 1;
    if (block.z < 0.0f)
        chunkZ -= 1;

    return glm::ivec3(chunkX, chunkY, chunkZ);
}

glm::ivec3 toChunkInternal(glm::ivec3 block)
{
    int x = 0;
    int y = 0;
    int z = 0;

    if (block.x >= 0.0f)
        x = (int)(block.x) % (CHUNK_SIZE);
    else
        x = (int)(abs(block.x)) % (CHUNK_SIZE) == 0 ? 0 : (CHUNK_SIZE)-(int)(abs(block.x)) % (CHUNK_SIZE);


    if (block.y >= 0.0f)
        y = (int)(block.y) % (CHUNK_SIZE);
    else
        y = (int)(abs(block.y)) % (CHUNK_SIZE) == 0 ? 0 : (CHUNK_SIZE)-(int)(abs(block.y)) % (CHUNK_SIZE);

    if (block.z >= 0.0f)
        z = (int)(block.z) % (CHUNK_SIZE);
    else
    {
        z = (int)(abs(block.z)) % (CHUNK_SIZE) == 0 ? 0 : (CHUNK_SIZE)-(int)(abs(block.z)) % (CHUNK_SIZE);
    }        


    return glm::ivec3(x, y, z);
}
