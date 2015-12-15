#ifndef BOSS_H
#define BOSS_H

#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "Weapons.h"
#include "Actor.h"

class Boss :public Actor
{
public:
    Boss() {}
    Boss(float x, float y) : Actor(x, y) {}
    Boss(Point2D p) : Actor(p) {}
    Boss(size_t w, size_t h)
    {
        SetPlayableArea(w, h);
    }
    bool IsAlive = true;
    double DamageCooldown = 500; // ms
    double CurrentDamageCooldown;
    float Speed; // pixel per milisecond (unscale)
    std::vector<boost::shared_ptr<Weapon>> Weapons;

    void SetPlayableArea(size_t w, size_t h)
    {
        playableWidth = w;
        playableHeight = h;
    }

    void UpdateCooldown(double elapsedGameTime)
    {
        for (std::vector<boost::shared_ptr<Weapon>>::size_type i = 0; i != Weapons.size(); i++)
        {
            Weapons.at(i)->CurrentCooldown = Weapons.at(i)->CurrentCooldown + elapsedGameTime;
        }
        CurrentDamageCooldown = CurrentDamageCooldown + elapsedGameTime;
    }

    void getDamage(size_t damage)
    {
        if (CurrentDamageCooldown >= DamageCooldown)
        {
            CurrentHealth = std::max(CurrentHealth - damage, (size_t)0);
            CurrentDamageCooldown = 0;
        }
        actionWhenGetDamaged(damage);
    }

    virtual void move(sf::Uint32 elapsed) = 0;
    virtual void attack() = 0;
    virtual void actionWhenGetDamaged(size_t damage) = 0;
    virtual void setupWeapons() = 0;
protected:
    size_t playableWidth, playableHeight;
};



#endif
