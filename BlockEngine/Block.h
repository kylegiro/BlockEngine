#pragma once

#include "Shader.h"
#include "Texture.h"

class Block
{
public:
	enum Type {
		AIR = 0,
		STONE = 1,
		DIRT = 2
	};

	Block();
	Block(Type type);
	Type getType();
	void setType(Type type);
	bool isOpaque();

	static std::string getAtlasKey(Block::Type type);

private:
	Type type;
};

