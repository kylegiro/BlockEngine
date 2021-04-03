#pragma once

#include "Shader.h"

class Quad
{
public:
	Quad();
	Quad(float r, float g, float b, float a);
	void render(Shader& shader);

private:
	unsigned int VBO, VAO, EBO;
	float r, g, b, a = 1.0f;
};

