#include "Chunk.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "ChunkManager.h"
#include "Util.h"
#include "WorldGen.h"


const float SIDE_SHADE = 0.8f;
const float BOT_SHADE = 0.8f;

Chunk::Chunk(int x, int y, int z, TextureAtlas& atlas, NoiseMap& heightMap, Camera& camera) 
    : x(x), y(y), z(z), atlas(atlas), numNeighbors(0), heightMap(heightMap), camera(camera)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 

    glGenVertexArrays(1, &dVAO);
    glGenBuffers(1, &dVBO);
    glGenBuffers(1, &dEBO);

    blocks = new Block * *[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        blocks[i] = new Block * [CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            blocks[i][j] = new Block[CHUNK_SIZE];
        }
    }
    generateTerrain();

    float dVerts[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 16.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -16.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    };

    unsigned int dIndices[] = {
        0, 1,
        0, 2,
        0, 3
    };

    glBindVertexArray(dVAO);

    glBindBuffer(GL_ARRAY_BUFFER, dVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dVerts) / sizeof(dVerts[0]) * sizeof(float), &dVerts[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(dIndices) / sizeof(dIndices[0]) * sizeof(unsigned int), &dIndices[0], GL_STREAM_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

Chunk::~Chunk()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            delete[] blocks[i][j];
        }
        delete[] blocks[i];
    }
    delete[] blocks;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteVertexArrays(1, &dVAO);
    glDeleteBuffers(1, &dVBO);
    glDeleteBuffers(1, &dEBO);

}

void Chunk::update(double dt)
{
}

void Chunk::render(Shader& shader)
{
    if (!shouldRender())
        return;

    // bind texture   
    atlas.bind();

    // transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(CHUNK_SIZE * (float)getX(), CHUNK_SIZE * (float)getY(), CHUNK_SIZE * (float)getZ()));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Chunk::renderDebug(Shader& shader)
{
    // transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(CHUNK_SIZE * (float)getX(), CHUNK_SIZE * (float)getY(), CHUNK_SIZE * (float)getZ()));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(dVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void Chunk::generateTerrain()
{
    int yWorld = getY() * CHUNK_SIZE;
    if (yWorld < 0) return;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                double xWorld = (double) getX() * CHUNK_SIZE + x;
                double zWorld = (double) getZ() * CHUNK_SIZE + z;
                yWorld = getY() * CHUNK_SIZE + y;

                double height = WorldGen::BASE_ELEVATION + (WorldGen::ELEVATION_VARIATION * heightMap.getValue(xWorld, zWorld));
                if (yWorld > height) continue;

                if(yWorld == (int) height)
                    setBlock(x, y, z, Block::Type::GRASS);
                else if(yWorld >= height - 3 && yWorld < height - 1)
                    setBlock(x, y, z, Block::Type::DIRT);
                else 
                    setBlock(x, y, z, Block::Type::STONE);
            }
        }
    }
}

Block Chunk::getBlock(int x, int y, int z)
{
    return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, Block::Type type)
{
    blocks[x][y][z].setType(type);
}

glm::vec3 Chunk::getCenterPos()
{
    return glm::vec3(x * CHUNK_SIZE + (CHUNK_SIZE/2), y * CHUNK_SIZE + (CHUNK_SIZE / 2), z * CHUNK_SIZE + (CHUNK_SIZE / 2));
}

void Chunk::rebuildMesh(ChunkManager& chunkManager)
{
    glNextIndex = 0;
    vertices.clear();
    indices.clear();    

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (blocks[x][y][z].getType() == Block::Type::AIR)
					continue;

                FaceRenderFlags faces = { true, true, true, true, true, true };

                if (x == 0)
                {                    
                    faces.xNeg = xNeg != nullptr && !xNeg->getBlock(CHUNK_SIZE - 1, y, z).isOpaque();
                    faces.xPos = !blocks[x + 1][y][z].isOpaque();
                }
                else if (x == CHUNK_SIZE - 1) 
                {                    
                    faces.xNeg = !blocks[x - 1][y][z].isOpaque();
                    faces.xPos = xPos != nullptr && !xPos->getBlock(0, y, z).isOpaque();
                }
                else
                {
                    faces.xNeg = !blocks[x - 1][y][z].isOpaque();
                    faces.xPos = !blocks[x + 1][y][z].isOpaque();
                }

                if (y == 0)
                {                    
                    faces.yNeg = yNeg != nullptr && !yNeg->getBlock(x, CHUNK_SIZE - 1, z).isOpaque();
                    faces.yPos = !blocks[x][y + 1][z].isOpaque();
                }
                else if (y == CHUNK_SIZE - 1)
                {                    
                    faces.yNeg = !blocks[x][y - 1][z].isOpaque();
                    faces.yPos = yPos != nullptr && !yPos->getBlock(x, 0, z).isOpaque();
                }
                else
                {
                    faces.yNeg = !blocks[x][y - 1][z].isOpaque();
                    faces.yPos = !blocks[x][y + 1][z].isOpaque();
                }

                if (z == 0)
                {                    
                    faces.zNeg = zNeg != nullptr && !zNeg->getBlock(x, y, CHUNK_SIZE - 1).isOpaque();
                    faces.zPos = !blocks[x][y][z + 1].isOpaque();
                }
                else if (z == CHUNK_SIZE - 1)
                {                    
                    faces.zNeg = !blocks[x][y][z - 1].isOpaque();
                    faces.zPos = zPos != nullptr && !zPos->getBlock(x, y, 0).isOpaque();
                }
                else
                {
                    faces.zNeg = !blocks[x][y][z - 1].isOpaque();
                    faces.zPos = !blocks[x][y][z + 1].isOpaque();
                }

                addBlockToMesh(x, y, z, faces, &blocks[x][y][z]);
			}			           
		}
	}

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STREAM_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);    

    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    updateBorderFlags();
    updateSurroundedFlag();

    if (needRebuildNeighbors)
    {        
        if (xNeg != nullptr)
            xNeg->setNeedsRebuild(true, false);
        if (xPos != nullptr)
            xPos->setNeedsRebuild(true, false);
        if (yNeg != nullptr)
            yNeg->setNeedsRebuild(true, false);
        if (yPos != nullptr)
            yPos->setNeedsRebuild(true, false);
        if (zNeg != nullptr)
            zNeg->setNeedsRebuild(true, false);
        if (zPos != nullptr)
            zPos->setNeedsRebuild(true, false);

        needRebuildNeighbors = false;
    }

    needRebuild = false;
}

bool Chunk::needsRebuild()
{
    return needRebuild;
}

void Chunk::setNeedsRebuild(bool rebuild, bool rebuildNeighbors)
{
    needRebuild = rebuild;
    needRebuildNeighbors = rebuildNeighbors;
}

int Chunk::getNumVertices()
{
    return vertices.size();
}

int Chunk::getNumIndices()
{
    return indices.size();
}

bool Chunk::operator<(const Chunk& other) const
{
    
    glm::ivec3 cameraChunkPos = chunkAtBlock(blockAt(camera.getPosition()));

    int distance = abs(cameraChunkPos.x - x) + abs(cameraChunkPos.y - y) + abs(cameraChunkPos.z - z);
    int otherDistance = abs(cameraChunkPos.x - other.getX()) + abs(cameraChunkPos.y - other.getY()) + abs(cameraChunkPos.z - other.getZ());

    return distance < otherDistance;
}

bool Chunk::closestToCamera(const Chunk* l, const Chunk* r)
{
    if ((*l) < (*r))
        return true;
    if ((*r) < (*l))
        return false;
    return (*l) < (*r);
}

int Chunk::getX() const
{
    return x;
}

int Chunk::getY() const
{
    return y;
}

int Chunk::getZ() const
{
    return z;
}

bool Chunk::shouldRender()
{
    return !surrounded;
}

void Chunk::updateBorderFlags()
{
    int xNegCount = 0;
    int xPosCount = 0;
    int yNegCount = 0;
    int yPosCount = 0;
    int zNegCount = 0;
    int zPosCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            if (blocks[x][y][0].isOpaque())
                zNegCount++;
            if (blocks[x][y][CHUNK_SIZE-1].isOpaque())
                zPosCount++;
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            if (blocks[x][0][z].isOpaque())
                yNegCount++;
            if (blocks[x][CHUNK_SIZE - 1][z].isOpaque())
                yPosCount++;
        }
    }

    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            if (blocks[0][y][z].isOpaque())
                xNegCount++;
            if (blocks[CHUNK_SIZE - 1][y][z].isOpaque())
                xPosCount++;
        }
    }

    xNegFull = false;
    xPosFull = false;
    yNegFull = false;
    yPosFull = false;
    zNegFull = false;
    zPosFull = false;

    int fullBorderSize = CHUNK_SIZE * CHUNK_SIZE;

    if (xNegCount == fullBorderSize)
        xNegFull = true;

    if (xPosCount == fullBorderSize)
        xPosFull = true;

    if (yNegCount == fullBorderSize)
        yNegFull = true;

    if (yPosCount == fullBorderSize)
        yPosFull = true;

    if (zNegCount == fullBorderSize)
        zNegFull = true;

    if (zPosCount == fullBorderSize)
        zPosFull = true;

}

void Chunk::updateSurroundedFlag()
{
    if (xNeg != nullptr && xNeg->xPosFull &&
        xPos != nullptr && xPos->xNegFull &&
        yNeg != nullptr && yNeg->yPosFull &&
        yPos != nullptr && yPos->yNegFull &&
        zNeg != nullptr && zNeg->zPosFull &&
        zPos != nullptr && zPos->zNegFull)
    {
        surrounded = true;
    }
    else 
    {
        surrounded = false;
    }
}

Chunk* Chunk::getXNeg()
{
    return xNeg;
}

Chunk* Chunk::getXPos()
{
    return xPos;
}

Chunk* Chunk::getYNeg()
{
    return yNeg;
}

Chunk* Chunk::getYPos()
{
    return yPos;
}

Chunk* Chunk::getZNeg()
{
    return zNeg;
}

Chunk* Chunk::getZPos()
{
    return zPos;
}

void Chunk::setXNeg(Chunk* chunk)
{
    xNeg = chunk;
}

void Chunk::setXPos(Chunk* chunk)
{
    xPos = chunk;
}

void Chunk::setYNeg(Chunk* chunk)
{
    yNeg = chunk;
}

void Chunk::setYPos(Chunk* chunk)
{
    yPos = chunk;
}

void Chunk::setZNeg(Chunk* chunk)
{
    zNeg = chunk;
}

void Chunk::setZPos(Chunk* chunk)
{
    zPos = chunk;
}

void Chunk::setNumNeighbors(int numNeighbors)
{
    this->numNeighbors = numNeighbors;
}

int Chunk::getNumNeighbors()
{
    return numNeighbors;
}

void Chunk::addBlockToMesh(int x, int y, int z, FaceRenderFlags faces, Block* block)
{    

    
    std::string textureKey = block->getTextureSide();
    UV uv = atlas.getUV(textureKey);

    if (faces.xNeg)
    {
        float left[] = {
            x, y, z,    uv.bottomLeft.x, uv.bottomLeft.y,       SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
            x, y, 1.0f + z,     uv.bottomRight.x, uv.bottomRight.y,     SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
            x, 1.0f + y, 1.0f + z,      uv.topRight.x, uv.topRight.y,           SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
            x, 1.0f + y, z,     uv.topLeft.x, uv.topLeft.y,             SIDE_SHADE, SIDE_SHADE, SIDE_SHADE
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(left[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.xPos)
    {
        float right[] = {
           1.0f + x, y, 1.0 + z,      uv.bottomLeft.x, uv.bottomLeft.y,        SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
           1.0f + x, y, z,     uv.bottomRight.x, uv.bottomRight.y,      SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
           1.0f + x, 1.0f + y, z,      uv.topRight.x, uv.topRight.y,            SIDE_SHADE, SIDE_SHADE, SIDE_SHADE,
           1.0f + x, 1.0f + y, 1.0f + z,       uv.topLeft.x, uv.topLeft.y,              SIDE_SHADE, SIDE_SHADE, SIDE_SHADE
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(right[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.zNeg)
    {
        float front[] = {
            1.0f + x, y, z,     uv.bottomLeft.x, uv.bottomLeft.y,           1.0f, 1.0f, 1.0f,
            x, y, z,    uv.bottomRight.x, uv.bottomRight.y,         1.0f, 1.0f, 1.0f,
            x, 1.0f + y, z,     uv.topRight.x, uv.topRight.y,               1.0f, 1.0f, 1.0f,
            1.0f + x, 1.0 + y, z,      uv.topLeft.x, uv.topLeft.y,               1.0f, 1.0f, 1.0f
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(front[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.zPos)
    {
        float back[] = {
            x, y, 1.0f + z,     uv.bottomLeft.x, uv.bottomLeft.y,       1.0f, 1.0f, 1.0f,
            1.0f + x, y, 1.0f + z,      uv.bottomRight.x, uv.bottomRight.y,     1.0f, 1.0f, 1.0f,
            1.0f + x, 1.0f + y, 1.0f + z,       uv.topRight.x, uv.topRight.y,           1.0f, 1.0f, 1.0f,
            x, 1.0f + y, 1.0f + z,      uv.topLeft.x, uv.topLeft.y,           1.0f, 1.0f, 1.0f
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(back[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }

    textureKey = block->getTextureBottom();
    uv = atlas.getUV(textureKey);

    if (faces.yNeg)
    {
        float bottom[] = {
            x, y, z,    uv.topLeft.x, uv.topLeft.y,              BOT_SHADE, BOT_SHADE, BOT_SHADE,
            1.0f + x, y, z,     uv.topRight.x, uv.topRight.y,            BOT_SHADE, BOT_SHADE, BOT_SHADE,
            1.0f + x, y, 1.0f + z,      uv.bottomRight.x, uv.bottomRight.y,      BOT_SHADE, BOT_SHADE, BOT_SHADE,
            x, y, 1.0f + z,     uv.bottomLeft.x, uv.bottomLeft.y,        BOT_SHADE, BOT_SHADE, BOT_SHADE
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(bottom[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }

    textureKey = block->getTextureTop();
    uv = atlas.getUV(textureKey);

    if (faces.yPos)
    {
        float top[] = {
            1.0f + x, 1.0f + y, z,      uv.topRight.x, uv.topRight.y,               1.0f, 1.0f, 1.0f,
            x, 1.0f + y, z,     uv.topLeft.x, uv.topLeft.y,               1.0f, 1.0f, 1.0f,
            x, 1.0f + y, 1.0f + z,      uv.bottomLeft.x, uv.bottomLeft.y,           1.0f, 1.0f, 1.0f,
            1.0f + x, 1.0f + y, 1.0f+ z,       uv.bottomRight.x, uv.bottomRight.y,         1.0f, 1.0f, 1.0f
        };

        for (int v = 0; v < 32; v++)
        {
            this->vertices.push_back(top[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }

}
