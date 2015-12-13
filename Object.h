#ifndef OBJECT_H
#define OBJECT_H

#include "Point2D.h"
#include "ObjectType.h"

class Object
{
public:
    Object(float x, float y)
    {
        Pos.X = x;
        Pos.Y = y;
    }
    Object(Point2D p)
    {
        Pos.X = p.X;
        Pos.Y = p.Y;
    }
    size_t width;
    size_t height;
    Point2D Pos;
    static size_t SCALE;
protected:
};

#endif
