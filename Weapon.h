#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include "Projectile.h"
class Team;
class Weapon
{
public:
    Weapon() {}
    std::string Name;
    std::string Desc;
    double Cooldown; // milisecond
    double CurrentCooldown;
    bool IsTesting = false;
    Team *team;
protected:
    virtual void FireProjectile(float startX, float startY, float desX, float desY) = 0;
    void Fire(float startX, float startY, float desX, float desY)
    {
        if (CurrentCooldown >= Cooldown)
        {
            FireProjectile(startX, startY, desX, desY);
        }
    }
    void Reset()
    {
        CurrentCooldown = 0;
    }
};



#endif
/*
public abstract class Weapon
{
public string Name { get; set; }
public float Cooldown { get; set; } // in miliseconds
public float CurrentCooldown { get; set; } = 1; // in miliseconds
public string Desc { get; set; }
public Projectile PrototypeProjectile { get; set; }
public bool IsTesting { get; set; } = false;

public Weapon()
{
}

public void Fire(float startX, float startY, float desX, float desY, bool IsFriendly)
{
if (CurrentCooldown > Cooldown)
{
CurrentCooldown = 0;
FireProjectile(startX, startY, desX, desY, IsFriendly);
}
}

public abstract void FireProjectile(float startX, float startY, float desX, float desY, bool IsFriendly);

public void Reset()
{
CurrentCooldown = 0;
}
}
*/
