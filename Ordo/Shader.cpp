#include "Shader.h"
#include <glad/glad.h>

Shader::Shader()
{
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
}
