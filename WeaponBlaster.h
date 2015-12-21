#ifndef WEAPONBLASTER_H_
#define WEAPONBLASTER_H_

#include "Weapon.h"

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
		float ShootPosY = startY;
		float rotation = (float)atan2(y - startY, x - startX) * 180 / boost::math::constants::pi<float>();
		ProjectileBlaster pro(startX, startY, rotation);
		Projectile::Projectiles.push_back(boost::make_shared<ProjectileBlaster>(pro));
	}
};
#endif
