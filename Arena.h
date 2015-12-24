#ifndef ARENA_H
#define ARENA_H

#include "Point2D.h"
#include "ArenaObjects.h"
#include "ObjectType.h"
#include <random>
#include <map>

class Arena
{
public:
    Arena();
    Arena(size_t playableWidth, size_t playableHeight, size_t jumpHeight);
    void SetupArenaObjects();
	ObjectType GetObjectAt(float x, float y);
	std::vector<Point2D> GetAllObjectPositionsInRec(float x, float y, float width, float height);
	Point2D GetNearestObjectPosition(float x, float y);
    std::map<Point2D, ObjectType>ArenaObjects;
    static const uint8_t OBJECT_SENT_PER_PACKAGE = (uint8_t)110;
    size_t PlayableWidth, PlayableHeight, JumpHeight;
private:
};

#endif
