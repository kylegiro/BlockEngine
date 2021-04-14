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
