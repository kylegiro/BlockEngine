#include "ChunkManager.h"

ChunkManager::ChunkManager(Shader& shader, Texture& texture) : shader(shader), texture(texture)
{
    ChunkCoord coord1 = { 0, 0, 0 };
    Chunk* chunk1 = new Chunk(0, 0, 0, texture);
    chunks[coord1] = chunk1;
    loadList.push_back(chunk1);
    setupList.push_back(chunk1);
    rebuildList.push_back(chunk1);

    ChunkCoord coord2 = { 0, 1, 0 };
    Chunk* chunk2 = new Chunk(0, 1, 0, texture);
    chunks[coord2] = chunk2;
    loadList.push_back(chunk2);
    setupList.push_back(chunk2);
    rebuildList.push_back(chunk2);
}

void ChunkManager::update(double dt)
{
    updateLoadList();
    updateSetupList();
    updateRebuildList();
    updateFlagsList();
    updateUnloadList();
    updateVisibilityList();
    updateRenderList();
}

void ChunkManager::render()
{    
    for (ChunkList::iterator iterator = renderList.begin(); iterator != renderList.end(); ++iterator) 
    {
        Chunk* chunk = (*iterator);
        chunk->render(shader);
    }
}

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

Chunk* ChunkManager::getChunk(int x, int y, int z)
{
    return nullptr;
}
