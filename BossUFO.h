#ifndef BOSSUFO_H
#define BOSSUFO_H

#include <algorithm>

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

    void attack() {}
    void getDamage(size_t damage) {};

    //public override void SetupWeapons()
    //{
    //    Weapons.Add(new WeaponBlaster());
    //    Weapons.Add(new WeaponBlaster());
    //    Weapons.Add(new WeaponBlaster());
    //    // Testing
    //    Weapons[0].CurrentCooldown = 100;
    //    Weapons[2].CurrentCooldown = 200;
    //    Weapons[1].CurrentCooldown = 50;
    //}

    //public override void Attack(Player player)
    //{
    //    if (!player.IsAlive) { return; }
    //    for (int i = 0; i < Weapons.Count; i++)
    //    {
    //        Weapons[i].Fire(PosX + (Width / 3 * i), PosY + (Height / 2), player.PosX, player.PosY + (player.CurrentHeight / 2), false);
    //    }
    //}

    //public override void GetDamage(int damage)
    //{
    //    if (CurrentDamageCooldown >= DamageCooldown)
    //    {
    //        CurrentHealth = Math.Max(CurrentHealth - damage, 0);
    //        CurrentDamageCooldown = 0;
    //    }
    //}


};


#endif
