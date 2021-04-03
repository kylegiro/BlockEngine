#pragma once

#include <string>

class Shader
{
public:
	Shader();

private:
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

