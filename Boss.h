#ifndef BOSS_H
#define BOSS_H

#include <boost/shared_ptr.hpp>
#include "Weapon.h"
#include "Object.h"
class Team;
class Boss :public Object
{
public:
    Boss(float x, float y) : Object(x, y) {}
    Boss(Point2D p) : Object(p) {}
    Boss() {}
    void setSize(size_t w, size_t h)
    {
        width = w;
        height = h;
    }
    Boss(size_t w, size_t h) 
    {
        SetPlayableArea(w, h);
    }
    size_t MaxHealth;
    size_t CurrentHealth;
    bool IsAlive = true;
    double DamageCooldown = 500; // ms
    double CurrentDamageCooldown;
    float Speed; // pixel per milisecond (unscale)
    std::vector<boost::shared_ptr<Weapon>> Weapons;
    Team* team;

    void UpdateCooldown(double elapsedGameTime)
    {
        for (std::vector<boost::shared_ptr<Weapon>>::size_type i = 0; i != Weapons.size(); i++)
        {
            Weapons.at(i)->CurrentCooldown = Weapons.at(i)->CurrentCooldown + elapsedGameTime;
        }
        CurrentDamageCooldown = CurrentDamageCooldown + elapsedGameTime;
    }

    virtual void move() = 0;
    virtual void attack() = 0;
    virtual void getDamage(size_t damage) = 0;

    void SetPlayableArea(size_t w, size_t h)
    {
        playableWidth = w;
        playableHeight = h;
    }
protected:
    size_t playableWidth, playableHeight;
};



#endif
