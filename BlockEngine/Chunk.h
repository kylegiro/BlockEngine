#pragma once

#include "Shader.h"
#include "Block.h"
#include "Texture.h"
#include "NoiseMap.h"
#include "Camera.h"

#include <vector>

const int CHUNK_SIZE = 16;

class ChunkManager;

struct FaceRenderFlags
{
	bool xNeg = true;
	bool xPos = true;
	bool yNeg = true;
	bool yPos = true;
	bool zNeg = true;
	bool zPos = true;
};

class Chunk
{
public:
	Chunk(int x, int y, int z, Texture& texture, NoiseMap& heightMap, Camera& camera);
	~Chunk();

	void update(double dt);
	void render(Shader& shader);
	void renderDebug(Shader& shader);

	void generateTerrain();

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block::Type type);

	glm::vec3 getCenterPos();

	int getX() const;
	int getY() const;
	int getZ() const;

	Chunk* getXNeg();
	Chunk* getXPos();
	Chunk* getYNeg();
	Chunk* getYPos();
	Chunk* getZNeg();
	Chunk* getZPos();
	void setXNeg(Chunk* chunk);
	void setXPos(Chunk* chunk);
	void setYNeg(Chunk* chunk);
	void setYPos(Chunk* chunk);
	void setZNeg(Chunk* chunk);
	void setZPos(Chunk* chunk);
	void setNumNeighbors(int numNeighbors);
	int getNumNeighbors();

	bool shouldRender();
	void updateBorderFlags();
	void updateSurroundedFlag();

	void rebuildMesh(ChunkManager& chunkManager);
	bool needsRebuild();
	void setNeedsRebuild(bool rebuild, bool rebuildNeighbors);

	int getNumVertices();
	int getNumIndices();

	bool operator<(const Chunk& other) const;
	static bool closestToCamera(const Chunk* l, const Chunk* r);

private:
	Block*** blocks;
	Texture& texture;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int glNextIndex = 0;

	void addBlockToMesh(int x, int y, int z, FaceRenderFlags faces);

	unsigned int VBO, VAO, EBO;
	unsigned int dVBO, dVAO, dEBO;

	// Neighbors
	int numNeighbors;
	Chunk* xNeg;
	Chunk* xPos;
	Chunk* yNeg;
	Chunk* yPos;
	Chunk* zNeg;
	Chunk* zPos;

	// Flags
	bool needRebuild, needRebuildNeighbors, empty, surrounded, xNegFull, xPosFull, yNegFull, yPosFull, zNegFull, zPosFull;

	int x, y, z;

	NoiseMap& heightMap;
	Camera& camera;
};

