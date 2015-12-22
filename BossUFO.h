#ifndef BOSSUFO_H
#define BOSSUFO_H

class BossUFO : public Boss
{
private:
	float weaponSpacing, weaponOffset;
public:
	BossUFO(size_t w, size_t h) :Boss(w, h)
	{
		id = ActorID::BOSS_UFO;
		MaxHealth = 300;
		CurrentHealth = MaxHealth;
		height = 15;
		width = 35;
		Speed = (float)0.045*(-1);
		setupWeapons();
	}

	void move(sf::Uint32 elapsed)
	{
		if (Pos.X >= (((float)playableWidth - width)) || Pos.X <= 0.f)
		{
			Speed = (-1) * Speed;
		}
		Pos.X = Pos.X + Speed*elapsed;
		Pos.X = std::min(Pos.X, (((float)playableWidth - width)));
		Pos.X = std::max(Pos.X, 0.f);
	}

	void attack()
	{
		if (!IsAlive) { return; }
		
		for (std::vector<boost::shared_ptr<Weapon>>::size_type i = 0; i != Weapons.size(); i++)
		{
			Weapons.at(i)->Fire(Pos.X + weaponOffset + (weaponSpacing * i), Pos.Y + (height / 2), 50, 70);
		}
	}

	void setupWeapons()
	{
		Weapons.clear();
		Weapons.push_back(boost::make_shared<WeaponBlaster>(10));
		Weapons.push_back(boost::make_shared<WeaponBlaster>(100));
		Weapons.push_back(boost::make_shared<WeaponBlaster>(299));
		weaponSpacing = (float)width / Weapons.size();
		weaponOffset = weaponSpacing / 2;
	}

	void actionWhenGetDamaged(size_t damage) {};
};


#endif
