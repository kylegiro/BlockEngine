#pragma once

#include "Shader.h"
#include "Block.h"
#include "Texture.h"

#include <vector>

const int CHUNK_SIZE = 16;

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
	Chunk(int x, int y, int z, Texture& texture);
	~Chunk();
	void update(double dt);
	void render(Shader& shader);
	void rebuildMesh();
	bool isLoaded();
	bool isSetup();
	void setup();
	void unload();
	void load();
	int getX();
	int getY();
	int getZ();
	bool shouldRender();

private:
	Block*** blocks;
	Texture& texture;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int glNextIndex = 0;

	void addBlockToMesh(int x, int y, int z, FaceRenderFlags faces);

	unsigned int VBO, VAO, EBO;

	bool loadedFlag = false;
	bool setupFlag = false;

	int x, y, z;
};

