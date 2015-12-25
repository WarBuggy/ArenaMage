#ifndef ARENABLOCK_H
#define ARENABLOCK_H
#include "ArenaObject.h"

class ArenaBlock : public ArenaObject
{
public:
    ArenaBlock(Point2D p) : ArenaObject(p)
    {
        setparam();
    }
    ArenaBlock(float x, float y) :ArenaObject(x, y)
    {
        setparam();
    }
    static const size_t MinBlockNum = 10;
    static const size_t MaxBlockNum = 20;
    static const size_t Chance = 30;
private:
    void setparam()
    {
        width = 1;
        height = 2;
        IsHeightScalable = false;
        IsStandable = true;
        IsPassable = false;
    }
};
#endif
