#include "ChunkManager.h"

#include <iostream>

ChunkManager::ChunkManager(Shader& shader, Texture& texture, Camera& camera) : shader(shader), texture(texture), camera(camera)
{
    loadChunk(0, 0, 0);
}

void ChunkManager::update(double dt)
{
    ChunkList updateList;
    ChunkCoordList loadList;
    ChunkList rebuildList;
    ChunkList unloadList;

    // Put all loaded chunks in the update list
    for (auto it = chunks.begin(); it != chunks.end(); it++)
    {
        Chunk* chunk = it->second;
        updateList.push_back(chunk);
    }    
    // Update chunks
    int chunksLoaded = 0;
    for (int i = 0; i < updateList.size(); i++)
    {
        Chunk* chunk = updateList[i];      

        if (chunk != nullptr)
        {
            int x = chunk->getX();
            int y = chunk->getY();
            int z = chunk->getZ();

            float xPos = (float)x * CHUNK_SIZE;
            float yPos = (float)y * CHUNK_SIZE;
            float zPos = (float)z * CHUNK_SIZE;

            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
            float distance = glm::distance(center, camera.getPosition());
            
            // Unload chunks beyond the load radius
            if (distance > UNLOAD_RADIUS)
            {
                unloadList.push_back(chunk);
            }
            else
            {
                if (chunksLoaded < MAX_CHUNKLOAD_PER_FRAME)
                {
                    // Load unloaded boundary chunks if they are within the load radius
                    if (chunk->getNumNeighbors() < 6)
                    {
                        if (chunk->getXNeg() == nullptr)
                        {
                            ChunkCoord coord = { x - 1, y, z };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());
                            //std::cout << i << " CHUNK(X,Y,Z)=(" << coord.x << "," << coord.y << "," << coord.z << ") distance=" << distance << "neighbors=" << chunk->getNumNeighbors() << std::endl;

                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                        if (chunk->getXPos() == nullptr)
                        {
                            ChunkCoord coord = { x + 1, y, z };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());

                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                        if (chunk->getYNeg() == nullptr)
                        {
                            ChunkCoord coord = { x, y - 1, z };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());

                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                        if (chunk->getYPos() == nullptr)
                        {
                            ChunkCoord coord = { x, y + 1, z };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());

                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                        if (chunk->getZNeg() == nullptr)
                        {
                            ChunkCoord coord = { x, y, z - 1 };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());

                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                        if (chunk->getZPos() == nullptr)
                        {
                            ChunkCoord coord = { x, y, z + 1 };

                            float xPos = (float)coord.x * CHUNK_SIZE;
                            float yPos = (float)coord.y * CHUNK_SIZE;
                            float zPos = (float)coord.z * CHUNK_SIZE;

                            glm::vec3 center = glm::vec3(xPos, yPos, zPos) + glm::vec3(CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f);
                            float distance = glm::distance(center, camera.getPosition());
                            if (distance <= LOAD_RADIUS) {
                                loadList.push_back(coord);
                                chunksLoaded++;
                            }
                        }
                    }
                }              
            }
        }
    }
    updateList.clear();

    // Load chunks
    for (int i = 0; i < loadList.size(); i++)
    {
        ChunkCoord coord = loadList[i];
        Chunk* chunk = getChunk(coord.x, coord.y, coord.z);        

        if (chunk == nullptr)
        {
            loadChunk(coord.x, coord.y, coord.z);
        }
        else
        {
            updateNeighbors(chunk, coord.x, coord.y, coord.z);
        }
    }
    loadList.clear();

    // Unload chunks
    for (int i = 0; i < unloadList.size(); i++)
    {
        Chunk* chunk = unloadList[i];

        unloadChunk(chunk);
    }
    unloadList.clear();
}

void ChunkManager::render()
{
    for (ChunkMap::iterator it = chunks.begin(); it != chunks.end(); it++)
    {
        Chunk* chunk = it->second;
        if(chunk != nullptr)
            chunk->render(shader);
    }
}


/*
void ChunkManager::updateLoadList()
{
    int numLoadedChunks = 0;
    ChunkList::iterator iterator;
    for (iterator = loadList.begin(); iterator != loadList.end() && (numLoadedChunks != MAX_CHUNKS); ++iterator) {
        Chunk* chunk = (*iterator);
        if (chunk->isLoaded() == false) {
            if (numLoadedChunks != MAX_CHUNKS) {
                chunk->load(); // Increase the chunks loaded count
                numLoadedChunks++;
                forceVisibilityUpdate = true;
            }
        }
    } // Clear the load list (every frame)
    loadList.clear();
}

void ChunkManager::updateSetupList()
{
    ChunkList::iterator iterator;
    for (iterator = setupList.begin(); iterator != setupList.end(); ++iterator) {
        Chunk* chunk = (*iterator);
        if (chunk->isLoaded() && !chunk->isSetup()) {
            chunk->setup();
            if (chunk->isSetup()) { // Only force the visibility update if we actually setup the chunk, some chunks wait in the pre-setup stage... 
                forceVisibilityUpdate = true;
            }
        }
    } // Clear the setup list (every frame)    
    setupList.clear();
}

void ChunkManager::updateRebuildList()
{
    // Rebuild any chunks that are in the rebuild chunk list     
    ChunkList::iterator iterator;
    int numRebuiltChunks = 0;
    for (iterator = rebuildList.begin(); iterator != rebuildList.end() && (numRebuiltChunks != MAX_CHUNKS); ++iterator) {
        Chunk* chunk = (*iterator);
        if (chunk->isLoaded() && chunk->isSetup()) {
            if (numRebuiltChunks != MAX_CHUNKS) {
                chunk->rebuildMesh(); // If we rebuild a chunk, add it to the list of chunks that need their render flags updated
                // since we might now be empty or surrounded
                flagsList.push_back(chunk); // Also add our neighbours since they might now be surrounded too (If we have neighbours)
                Chunk* xNeg = getChunk(chunk->getX() - 1, chunk->getY(), chunk->getZ());
                Chunk* xPos = getChunk(chunk->getX() + 1, chunk->getY(), chunk->getZ());
                Chunk* yNeg = getChunk(chunk->getX(), chunk->getY() - 1, chunk->getZ());
                Chunk* yPos = getChunk(chunk->getX(), chunk->getY() + 1, chunk->getZ());
                Chunk* zNeg = getChunk(chunk->getX(), chunk->getY(), chunk->getZ() - 1);
                Chunk* zPos = getChunk(chunk->getX(), chunk->getY(), chunk->getZ() + 1);
                if (xNeg != NULL) flagsList.push_back(xNeg);
                if (xPos != NULL) flagsList.push_back(xPos);
                if (yNeg != NULL) flagsList.push_back(yNeg);
                if (yPos != NULL) flagsList.push_back(yPos);
                if (zNeg != NULL) flagsList.push_back(zNeg);
                if (zPos != NULL) flagsList.push_back(zPos); // Only rebuild a certain number of chunks per frame
                numRebuiltChunks++;
                forceVisibilityUpdate = true;
            }
        }
    }
    // Clear the rebuild list
    rebuildList.clear();
}

void ChunkManager::updateFlagsList()
{
}

void ChunkManager::updateUnloadList()
{
    ChunkList::iterator iterator;
    for (iterator = unloadList.begin(); iterator != unloadList.end(); ++iterator) {
        Chunk* chunk = (*iterator);
        if (chunk->isLoaded()) {
            chunk->unload();
            unloadChunk(chunk);
            forceVisibilityUpdate = true;
        }
    } // Clear the unload list (every frame)    
    unloadList.clear();
}

void ChunkManager::updateVisibilityList()
{
    visibilityList.clear();
    // TODO frustum culling
    for (std::pair<ChunkCoord, Chunk*> pair : chunks)
    {
        Chunk* chunk = pair.second;
        visibilityList.push_back(chunk);
    }
}

void ChunkManager::updateRenderList()
{
    // Clear the render list each frame BEFORE we do our tests to see what chunks should be rendered     
    renderList.clear();
    ChunkList::iterator iterator;
    for (iterator = visibilityList.begin(); iterator != visibilityList.end(); ++iterator) {
        Chunk* chunk = (*iterator);
        if (chunk != NULL) {
            if (chunk->isLoaded() && chunk->isSetup()) {
                if (chunk->shouldRender()) // Early flags check so we don't always have to do the frustum check... 
                {
                    renderList.push_back(chunk);
                }
            }
        }
    }
}
*/

Chunk* ChunkManager::getChunk(int x, int y, int z)
{
    ChunkCoord coord = { x, y, z };
    ChunkMap::iterator it = chunks.find(coord);
    if (it != chunks.end())
    {
        Chunk* chunk = chunks[coord];
        return chunks[coord];
    }
    return nullptr;
}

void ChunkManager::loadChunk(int x, int y, int z)
{
    ChunkCoord coord = { x, y, z };
    Chunk* chunk = new Chunk(x, y, z, texture);
    chunks[coord] = chunk;
    chunk->rebuildMesh();

    updateNeighbors(chunk, x, y, z);
}

void ChunkManager::unloadChunk(Chunk* chunk)
{    
    ChunkCoord coord = { chunk->getX(), chunk->getY(), chunk->getZ() };

    Chunk* chunkXNeg = getChunk(coord.x - 1, coord.y, coord.z);
    Chunk* chunkXPos = getChunk(coord.x + 1, coord.y, coord.z);
    Chunk* chunkYNeg = getChunk(coord.x, coord.y - 1, coord.z);
    Chunk* chunkYPos = getChunk(coord.x, coord.y + 1, coord.z);
    Chunk* chunkZNeg = getChunk(coord.x, coord.y, coord.z - 1);
    Chunk* chunkZPos = getChunk(coord.x, coord.y, coord.z + 1);

    if (chunkXNeg)
    {
        if (chunkXNeg->getXPos())
        {
            chunkXNeg->setNumNeighbors(chunkXNeg->getNumNeighbors() - 1);
            chunkXNeg->setXPos(nullptr);
        }
    }
    if (chunkXPos)
    {
        if (chunkXPos->getXNeg())
        {
            chunkXPos->setNumNeighbors(chunkXPos->getNumNeighbors() - 1);
            chunkXPos->setXNeg(nullptr);
        }
    }
    if (chunkYNeg)
    {
        if (chunkYNeg->getYPos())
        {
            chunkYNeg->setNumNeighbors(chunkYNeg->getNumNeighbors() - 1);
            chunkYNeg->setYPos(nullptr);
        }
    }
    if (chunkYPos)
    {
        if (chunkYPos->getYNeg())
        {
            chunkYPos->setNumNeighbors(chunkYPos->getNumNeighbors() - 1);
            chunkYPos->setYNeg(nullptr);
        }
    }
    if (chunkZNeg)
    {
        if (chunkZNeg->getZPos())
        {
            chunkZNeg->setNumNeighbors(chunkZNeg->getNumNeighbors() - 1);
            chunkZNeg->setZPos(nullptr);
        }
    }
    if (chunkZPos)
    {
        if (chunkZPos->getZNeg())
        {
            chunkZPos->setNumNeighbors(chunkZPos->getNumNeighbors() - 1);
            chunkZPos->setZNeg(nullptr);
        }
    }

    std::unordered_map<ChunkCoord, Chunk*>::iterator it = chunks.find(coord);
    if (it != chunks.end())
        chunks.erase(coord);
    chunk->unload();
    delete chunk;
}

void ChunkManager::updateNeighbors(Chunk* chunk, int x, int y, int z)
{
    chunk->setNumNeighbors(0);

    Chunk* chunkXNeg = getChunk(x - 1, y, z);
    Chunk* chunkXPos = getChunk(x + 1, y, z);
    Chunk* chunkYNeg = getChunk(x, y - 1, z);
    Chunk* chunkYPos = getChunk(x, y + 1, z);
    Chunk* chunkZNeg = getChunk(x, y, z - 1);
    Chunk* chunkZPos = getChunk(x, y, z + 1);

    if (chunkXNeg)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setXNeg(chunkXNeg);
        if (chunkXNeg->getXPos() == nullptr)
        {
            chunkXNeg->setNumNeighbors(chunkXNeg->getNumNeighbors() + 1);
            chunkXNeg->setXPos(chunk);
        }
    }
    if (chunkXPos)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setXPos(chunkXPos);
        if (chunkXPos->getXNeg() == nullptr)
        {
            chunkXPos->setNumNeighbors(chunkXPos->getNumNeighbors() + 1);
            chunkXPos->setXNeg(chunk);
        }
    }
    if (chunkYNeg)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setYNeg(chunkYNeg);
        if (chunkYNeg->getYPos() == nullptr)
        {
            chunkYNeg->setNumNeighbors(chunkYNeg->getNumNeighbors() + 1);
            chunkYNeg->setYPos(chunk);
        }
    }
    if (chunkYPos)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setYPos(chunkYPos);
        if (chunkYPos->getYNeg() == nullptr)
        {
            chunkYPos->setNumNeighbors(chunkYPos->getNumNeighbors() + 1);
            chunkYPos->setYNeg(chunk);
        }
    }
    if (chunkZNeg)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setZNeg(chunkZNeg);
        if (chunkZNeg->getZPos() == nullptr)
        {
            chunkZNeg->setNumNeighbors(chunkZNeg->getNumNeighbors() + 1);
            chunkZNeg->setZPos(chunk);
        }
    }
    if (chunkZPos)
    {
        chunk->setNumNeighbors(chunk->getNumNeighbors() + 1);
        chunk->setZPos(chunkZPos);
        if (chunkZPos->getZNeg() == nullptr)
        {
            chunkZPos->setNumNeighbors(chunkZPos->getNumNeighbors() + 1);
            chunkZPos->setYNeg(chunk);
        }
    }
}
