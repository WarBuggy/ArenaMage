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
		Rotation = rotation;
		SpeedY = sin(Rotation)*Speed;
		SpeedX = cos(Rotation)*Speed;
	}

	void Init()
	{
		Name = "Blaster lazer";
		Speed = (float)0.09;
		width = 6;
		height = 1;
		PrevX = Pos.X;
		PrevY = Pos.Y;
	}

	void Move(sf::Uint32 elapsed)
	{
		PrevX = Pos.X;
		PrevY = Pos.Y;
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
			Point2D projected = GetProjectedShape();
			float topX = projected.X;
			float topY = projected.Y;
			float X = std::min(topX, PrevX);
			float Y = std::min(topY, PrevY);
			float width = abs(topX - PrevX);
			float height = abs(topY - PrevY);

			std::vector<Point2D> points = arena.GetAllObjectPositionsInRec(X, Y, width, height);
			if (points.size() < 1) { return; }
			for (std::vector<Point2D>::iterator it = points.begin(); it != points.end(); ++it)
			{
				ObjectType type = arena.ArenaObjects[*it];
				float objectWidth = 1;
				bool IsPassable = true;
				if (type == ObjectType::ARENA_BLOCK)
				{
					IsPassable = false;
				}
				if (!IsPassable && CheckIntersect(PrevX, PrevY, topX, topY, it->X, it->Y, it->X + objectWidth, it->Y))
				{
					//ScreenPlay.AddImpact(new Vector2(obj.PosX, obj.PosY));
					ToBeRemove = true;
					break;
				}
			}
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

/*

public override void DetectCollisionWithArenaObject(Arena arena)
{
if (ToBeRemove) { return; }
Vector2 projected = GetProjectedMoveVector();
float topX = projected.X;
float topY = projected.Y;
float X = Math.Min(topX, PosX);
float Y = Math.Min(topY, PosY);
float width = Math.Abs(topX - PosX);
float height = Math.Abs(topY - PosY);

List<ArenaObject> objs = arena.GetAllObjectsInRec(X, Y, width, height);
if (objs.Count < 1) { return; }
foreach (ArenaObject obj in objs)
{
if (CheckIntersect(PosX, PosY, topX, topY, obj.PosX, obj.PosY, obj.PosX + obj.Width, obj.PosY) && !obj.IsPassable)
{
ScreenPlay.AddImpact(new Vector2(obj.PosX, obj.PosY));
ToBeRemove = true;
break;
}
}
}

public override void DetectCollisionWithBoss(Boss boss)
{
if (ToBeRemove) { return; }
if (!IsFriendly) { return; } // if this is a projectile from hostile object, do nothing
Vector2 projected = GetProjectedMoveVector();
float topX = projected.X;
float topY = projected.Y;
if (CheckIntersect(PosX, PosY, topX, topY, boss.PosX, boss.PosY, boss.PosX + boss.Width, boss.PosY)) // top
{
boss.GetDamage(Damage);
ToBeRemove = true;
}
else if (CheckIntersect(PosX, PosY, topX, topY, boss.PosX, boss.PosY, boss.PosX, boss.PosY + boss.Height)) // left
{
boss.GetDamage(Damage);
ToBeRemove = true;
}
else if (CheckIntersect(PosX, PosY, topX, topY, boss.PosX + boss.Width, boss.PosY, boss.PosX + boss.Width, boss.PosY + boss.Height)) // right
{
boss.GetDamage(Damage);
ToBeRemove = true;
}
else if (CheckIntersect(PosX, PosY, topX, topY, boss.PosX, boss.PosY + boss.Height, boss.PosX + boss.Width, boss.PosY + boss.Height)) // bottom
{
boss.GetDamage(Damage);
ToBeRemove = true;
}
else if (new RotatedRectangle(new Rectangle((int)PosX, (int)PosY, (int)RecWidth, (int)RecHeight), Rotation).Intersects(boss.GetRec()))
{
boss.GetDamage(Damage);
ToBeRemove = true;
}
}

public override void DetectCollisionWithPlayer(Player player)
{
if (ToBeRemove) { return; }
if (IsFriendly) { return; } // if this is a friendly projectile, do nothing
Vector2 projected = GetProjectedMoveVector();
float topX = projected.X;
float topY = projected.Y;

// check intersect with player top
if (CheckIntersect(PosX, PosY, topX, topY, player.PosX, player.PosY, player.PosX + player.CurrentWidth, player.PosY))
{
player.GetDamage(Damage);
ToBeRemove = true;
}
// check intersect with left side
else if (CheckIntersect(PosX, PosY, topX, topY, player.PosX, player.PosY, player.PosX, player.PosY + player.CurrentHeight))
{
player.GetDamage(Damage);
ToBeRemove = true;
}
// check intersect with right side
else if (CheckIntersect(PosX, PosY, topX, topY, player.PosX + player.CurrentWidth, player.PosY, player.PosX + player.CurrentWidth, player.PosY + player.CurrentHeight))
{
player.GetDamage(Damage);
ToBeRemove = true;
}
else if (new RotatedRectangle(new Rectangle((int)PosX, (int)PosY, (int)RecWidth, (int)RecHeight), Rotation).Intersects(player.GetRectangle()))
{
player.GetDamage(Damage);
ToBeRemove = true;
}
}
*/