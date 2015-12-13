#ifndef ARENAOBJECT_H
#define ARENAOBJECT_H

#include "Object.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"


class ArenaObject : public Object
{
public:
    ArenaObject(Point2D p) : Object(p) {}
    ArenaObject(float x, float y) : Object(x, y) {}
    bool IsHeightScalable = false;
    bool IsStandable = true;
    bool IsPassable = true;
    sf::Texture texture;
    sf::Sprite sprite;
protected:
private:
    bool operator ==(const ArenaObject &obj) const
    {
        return ((obj.Pos.X == Pos.X) && (obj.Pos.Y == Pos.Y));
    }
};

#endif

