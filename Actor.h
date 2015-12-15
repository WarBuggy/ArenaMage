#ifndef ACTOR_H
#define ACTOR_H

#include "Object.h"
#include "ActorID.h"
// used for drawing
class Actor : public Object 
{
public:
    ActorID id;
    Actor(float x, float y) : Object(x, y) {}
    Actor(Point2D p) : Object(p) {}
    Actor() {}
    void setSize(size_t w, size_t h)
    {
        width = w;
        height = h;
    }
    size_t MaxHealth;
    size_t CurrentHealth;
};

#endif

