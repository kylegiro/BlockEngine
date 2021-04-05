#pragma once

#include "Shader.h"
#include "Texture.h"

class Block
{
public:
	Block(float r, float g, float b, float a, Texture& texture);
	void render(Shader& shader);

private:
	Texture& texture;
	unsigned int VBO, VAO, EBO;
	float r, g, b, a;
};

