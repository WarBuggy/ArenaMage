#ifndef PROJECTILEBLASTER_H
#define PROJECTILEBLASTER_H

#include "Projectile.h"

class ProjectileBlaster :public Projectile
{
public:
	ProjectileBlaster() : Projectile()
	{
		Init();
	}
	ProjectileBlaster(float x, float y, float rotation) : Projectile(x, y)
	{
		Init();
		Name = "Blaster lazer";
		Color = sf::Color(255, 0, 0);
		Speed = (float)0.3;
		width = 8;
		height = 1;
		Rotation = rotation;
		SpeedY = sin(Rotation)*Speed;
		SpeedX = cos(Rotation)*Speed;
	}

	void Init()
	{
		Name = "Blaster lazer";
		Color = sf::Color(255, 0, 0);
		Speed = (float)0.08;
		width = 8;
		height = 1;
	}

	void Move(sf::Uint32 elapsed)
	{
		Pos.X = Pos.X + SpeedX*elapsed;
		Pos.Y = Pos.Y + SpeedY*elapsed;
	}

	void DetectCollisionWithArenaObject(Arena &arena)
	{
		if (Pos.X < 0 || Pos.X > arena.PlayableWidth || Pos.Y < 0 || Pos.Y > arena.PlayableHeight)
		{
			ToBeRemove = true;
		}
		else
		{

		}
	}
	void DetectCollisionWitObject(Object object) {}
	void CreateDataForClients(sf::Packet &p)
	{
		p << (sf::Uint8) ObjectType::PROJECTILE_BLASTER << Pos.X << Pos.Y << Rotation;
	}

	sf::IntRect GetTextureCoord()
	{
		sf::IntRect rec(0, 0, 1, 10);
		return rec;
	}
};



#endif
