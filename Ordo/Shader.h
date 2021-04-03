#pragma once

#include <string>

class Shader
{
public:
	Shader(std::string vertexPath, std::string fragmentPath);

private:
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

