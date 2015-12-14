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




};



#endif
