#ifndef BOSSUFO_H
#define BOSSUFO_H

class BossUFO : public Boss
{
public:
    BossUFO(size_t w, size_t h) :Boss(w, h)
    {
        id = ActorID::BOSS_UFO;
        MaxHealth = 300;
        CurrentHealth = MaxHealth;
        height = 15;
        width = 35;
        Speed = (float)0.2;
        setupWeapons();
    }

    void move(sf::Uint32 elapsed)
    {
        Pos.X = std::min(Pos.X, (((float)playableWidth - width))*SCALE);
        Pos.X = std::max(Pos.X, 0.f);
        if (Pos.X == (((float)playableWidth - width))*SCALE || Pos.X == 0.f)
        {
            Speed = (-1) * Speed;
        }
        Pos.X = Pos.X + Speed*elapsed;
    }

    void attack()
    {
        if (!IsAlive) { return; }
        for (std::vector<boost::shared_ptr<Weapon>>::size_type i = 0; i != Weapons.size(); i++)
        {
            Weapons.at(i)->Fire((Pos.X + width / 3 * i)*SCALE, (Pos.Y + height / 2)*SCALE, 200, 200);
        }
    }

    void setupWeapons()
    {
        Weapons.clear();
        Weapons.push_back(boost::make_shared<WeaponBlaster>(100));
        Weapons.push_back(boost::make_shared<WeaponBlaster>(150));
        Weapons.push_back(boost::make_shared<WeaponBlaster>(200));
    }
    
    void actionWhenGetDamaged(size_t damage) {};
};


#endif
