#include "Block.h"

Block::Block() : type(STONE)
{
}

Block::Block(Type type) : type(type)
{
}

Block::Type Block::getType()
{
    return type;
}

bool Block::isOpaque()
{
    return type != AIR;
}
