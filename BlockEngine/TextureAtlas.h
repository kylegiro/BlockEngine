#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Texture.h"

struct AtlasTexture
{
	int x, y, width, height;
};

struct UV
{
	glm::vec2 bottomLeft, bottomRight, topLeft, topRight;
};

class TextureAtlas
{
public:
	TextureAtlas(unsigned int width, unsigned int height, std::string path);
	UV getUV(std::string key);
	void bind();

private:
	unsigned int width, height;
	std::unordered_map<std::string, AtlasTexture> map;
	Texture texture;

	UV toUV(AtlasTexture& tex);
};

