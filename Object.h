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
    void SetWidth(int w)
    {
        width = w;
    }
    void SetHeight(int h)
    {
        height = h;
    }
    size_t GetWidth()
    {
        return width;
    }
    size_t GetHeight()
    {
        return height;
    }
    void SetPoint(Point2D p)
    {
        Pos.X = p.X;
        Pos.Y = p.Y;
    }
    Point2D GetPoint()
    {
        return Pos;
    }
protected:
    size_t width;
    size_t height;
    Point2D Pos;
};

#endif
