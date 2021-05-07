#pragma once

#include "Shader.h"
#include "Texture.h"

class Block
{
public:
	enum Type {
		AIR = 0,
		STONE = 1,
		DIRT = 2,
		GRASS = 3
	};

	Block();
	Block(Type type);
	Type getType();
	void setType(Type type);
	bool isOpaque();

	std::string getTextureSide();
	std::string getTextureTop();
	std::string getTextureBottom();	

private:
	Type type;
};

