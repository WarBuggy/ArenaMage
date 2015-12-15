#ifndef PROJECTILEBLASTER_H
#define PROJECTILEBLASTER_H

#include "Projectile.h"

class ProjectileBlaster :public Projectile
{
public:
    ProjectileBlaster() : Projectile()
    {
        Color = sf::Color(255, 0, 0);
    }
    ProjectileBlaster(float x, float y) : Projectile(x, y)
    {
        ProjectileBlaster();
    }

    void Move() {}
    void DetectCollisionWithArenaObject(Arena arena) {}
    void DetectCollisionWitObject(Object object) {}
};



#endif
