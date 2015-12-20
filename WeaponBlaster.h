#ifndef WEAPONBLASTER_H_
#define WEAPONBLASTER_H_

#include "Weapon.h"
//#include "ServerSFML.h"

class WeaponBlaster : public Weapon
{
public:
    WeaponBlaster()
    {
        Name = "Blaster";
        Desc = "Fast speed, reliable damage. Your first choice on the path to fame.";
        Cooldown = 300;
    }

    WeaponBlaster(size_t initialCurrentCooldown)
    {
        WeaponBlaster();
        CurrentCooldown = initialCurrentCooldown;
    }

    void FireProjectile(float startX, float startY, float x, float y)
    {
        ProjectileBlaster pro(startX, startY);
        float ShootPosY = startY;
        double angle = atan2(y - startY, x - startX);
        pro.SpeedY = (float)sin(angle) * pro.Speed;
        pro.SpeedX = (float)cos(angle) * pro.Speed;
        pro.Rotation = (float)angle;
        //ServerSFML::Projectiles.push_back(boost::make_shared<ProjectileBlaster>(pro));
    }


};
#endif
