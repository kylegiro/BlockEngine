#pragma once

#include "Shader.h"
#include "Block.h"
#include "Texture.h"

#include <vector>

const int SIZE = 16;

class Chunk
{
public:
	Chunk(Texture& texture);
	~Chunk();
	void update(double dt);
	void render(Shader& shader);
	void rebuildMesh();

private:
	Block*** blocks;
	Texture& texture;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int nextIndex = 0;

	void addBlockToMesh(int x, int y, int z);

	unsigned int VBO, VAO, EBO;

};

