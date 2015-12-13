#ifndef ARENAOBJECT_H
#define ARENAOBJECT_H

#include "Object.h"

class ArenaObject : public Object
{
public:
    ArenaObject(Point2D p) : Object(p) {}
    ArenaObject(float x, float y) : Object(x, y) {}
    bool GetIsStandble()
    {
        return IsStandable;
    }
    bool GetIsPassable()
    {
        return IsPassable;
    }

    bool operator ==(const ArenaObject &obj) const
    {
        return ((obj.Pos.X == Pos.X) && (obj.Pos.Y == Pos.Y));
    }
protected:
    bool IsStandable = true;
    bool IsPassable = true;
    bool IsHeightScalable = false;
};

#endif

