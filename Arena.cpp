#include "Arena.h"
extern std::mt19937 rng;

Arena::Arena()
{
}

Arena::Arena(size_t playableWidth, size_t playableHeight, size_t jumpHeight)
{
	PlayableWidth = playableWidth;
	PlayableHeight = playableHeight;
	JumpHeight = jumpHeight;
}

void Arena::SetupArenaObjects()
{
	size_t rows = PlayableHeight / JumpHeight;
	ArenaObjects.clear();
	for (size_t i = 1; i < rows; i++)
	{
		for (size_t j = 0; j < PlayableWidth; j++)
		{
			std::uniform_int_distribution<size_t> uni(0, 100);
			size_t chance = uni(rng);
			if (chance <= ArenaBlock::Chance)
			{
				std::uniform_int_distribution<size_t> repeatRNG(ArenaBlock::MinBlockNum, ArenaBlock::MinBlockNum);
				size_t repeat = repeatRNG(rng);
				size_t k;
				for (k = j; (k < j + repeat) && (k < PlayableWidth); k++)
				{
					Point2D p = { (float)k, (float)(PlayableHeight - (i*JumpHeight)) };
					ArenaObjects.insert(std::pair<Point2D, ObjectType>(p, ObjectType::ARENA_BLOCK));
				}
				j = k - 1;
			}
			else
			{
				std::uniform_int_distribution<size_t> repeatRNG(ArenaPlatform::MinBlockNum, ArenaPlatform::MinBlockNum);
				size_t repeat = repeatRNG(rng);
				size_t k;
				for (k = j; (k < j + repeat) && (k < PlayableWidth); k++)
				{
					Point2D p = { (float)k, (float)(PlayableHeight - (i*JumpHeight)) };
					ArenaObjects.insert(ArenaObjects.begin(), std::pair<Point2D, ObjectType>(p, ObjectType::ARENA_PLATFORM));
				}
				j = k - 1;
			}
		}
	}
}

ObjectType Arena::GetObjectAt(float x, float y)
{
	Point2D key = Point2D{ x, y };
	if (ArenaObjects.find(key) != ArenaObjects.end())
	{
		return ArenaObjects[key];
	}
	return ObjectType::NOT_DEFINE;
}

std::vector<Point2D> Arena::GetAllObjectPositionsInRec(float x, float y, float width, float height)
{
	std::vector<Point2D> points;
	size_t rows = (size_t)(height + 2);
	size_t cols = (size_t)(width + 2);
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			Point2D aPoint = GetNearestObjectPosition(x + col, y + row);
			ObjectType objType = GetObjectAt(aPoint.X, aPoint.Y);
			if (objType != ObjectType::NOT_DEFINE && (std::find(points.begin(), points.end(), aPoint) == points.end()))
			{
				points.push_back(aPoint);
			}
		}
	}
	return points;
}

Point2D Arena::GetNearestObjectPosition(float x, float y)
{
	Point2D result;
	result.X = (float)((size_t)x);
	result.Y = (float)((size_t)y);
	return result;
}
