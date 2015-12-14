#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include <cmath>
#include<SFML/Graphics/Color.hpp>

class Team;
class Projectile :public Object
{
public:
    Projectile(float x, float y) : Object(x, y) {}
    Projectile(Point2D p) : Object(p) {}
    Projectile() {}
    std::string Name;
    size_t Damage;
    float Speed; // pixel per milisecond
    float SpeedX, SpeedY;
    float Rotation;
    bool ToBeRemove = false;
    float TimeActive = 0, timeSinceFire;// in miliseconds, default 0, no active timer. Active timer used for weapon like flame thrower
    Team* team;
    sf::Color Color;

    bool CheckIntersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y)
    {
        float s1_x, s1_y, s2_x, s2_y;
        s1_x = p1_x - p0_x; s1_y = p1_y - p0_y;
        s2_x = p3_x - p2_x; s2_y = p3_y - p2_y;

        float s, t;
        s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        {
            // Collision detected
            //if (i_x != NULL)
            //    *i_x = p0_x + (t * s1_x);
            //if (i_y != NULL)
            //    *i_y = p0_y + (t * s1_y);
            //return 1;
            return true;
        }
        return false; // No collision
    }

    Point2D GetProjectedMoveVector()
    {
        float topX = Pos.X + SpeedX;
        float topY = Pos.Y + SpeedY;
        return Point2D{ topX, topY };
    }

    Point2D GetProjectedShapeVector()
    {
        float topX = Pos.X + cos(Rotation) * width;
        float topY = Pos.Y + sin(Rotation) * width;
        return Point2D{ topX, topY };
    }

    virtual void Move() = 0;
    virtual void DetectCollisionWithArenaObject(Arena arena) = 0;
    virtual void DetectCollisionWitObject(Object object) = 0;
};
#endif
