#include "Block.h"

Block::Block() : type(AIR)
{
}

Block::Block(Type type) : type(type)
{
}

Block::Type Block::getType()
{
    return type;
}

void Block::setType(Type type)
{
    this->type = type;
}

bool Block::isOpaque()
{
    return type != AIR;
}

std::string Block::getAtlasKey(Block::Type type)
{
    switch (type)
    {
    case STONE:
        return "stone";
    case DIRT:
        return "dirt";
    }
}