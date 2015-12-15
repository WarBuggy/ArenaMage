#ifndef BOSS_H
#define BOSS_H

#include <boost/shared_ptr.hpp>
#include "Weapon.h"
#include "Actor.h"
class Team;
class Boss :public Actor
{
public:
    Boss(float x, float y) : Actor(x, y) {}
    Boss(Point2D p) : Actor(p) {}
    Boss() {}
    Boss(size_t w, size_t h) 
    {
        SetPlayableArea(w, h);
    }
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

    virtual void move(sf::Uint32 elapsed) = 0;
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
