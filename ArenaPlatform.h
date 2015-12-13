#ifndef ARENAPLATFORM_H
#define ARENAPLATFORM_H

#include "ArenaObject.h"

class ArenaPlatform : public ArenaObject
{
public:
    ArenaPlatform(Point2D p) : ArenaObject(p)
    {
        setparam();
    }
    ArenaPlatform(float x, float y) :ArenaObject(x, y)
    {
        setparam();
    }
    static const size_t Chance = 100;
    static const size_t MinBlockNum = 10;
    static const size_t MaxBlockNum = 20;
private:
    void setparam()
    {
        width = 1;
        height = 2;
        IsHeightScalable = false;
        IsStandable = true;
        IsPassable = true;
    }
};

#endif
