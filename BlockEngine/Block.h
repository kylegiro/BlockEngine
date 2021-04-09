#pragma once

#include "Shader.h"
#include "Texture.h"

class Block
{
public:
	enum Type {
		AIR = 0,
		STONE = 1
	};

	Block();
	Block(Type type);
	Type getType();
	bool isOpaque();

private:
	Type type;
};

