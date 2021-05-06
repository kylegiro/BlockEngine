#pragma once

#include <string>

class Texture
{
public:
	Texture(std::string path);
	void bind();

private:
	unsigned int texture;
};

