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

std::string Block::getTextureSide()
{
    switch (type)
    {
    case STONE:
        return "stone";
    case DIRT:
        return "dirt";
    case GRASS:
        return "grass_side";
    default:
        return "stone";
    }
}

std::string Block::getTextureTop()
{
    if (type != GRASS)
        return getTextureSide();
    return "grass_top";
}

std::string Block::getTextureBottom()
{
    if (type != GRASS)
        return getTextureSide();
    return "dirt";
}
