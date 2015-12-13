#ifndef ARENA_H
#define ARENA_H

#include "Point2D.h"
#include <map>
#include "ArenaObjects.h"
#include <random>

class Arena
{
public:
    Arena();
    Arena(size_t playableWidth, size_t playableHeight, size_t jumpHeight);
    void SetupArenaObjects();
    void SetWidth(size_t width);
    void SetHeight(size_t height);
    void SetJumpHeight(size_t jumpHeight);
    std::map<Point2D, ObjectType>ArenaObjects;
private:
    size_t PlayableWidth, PlayableHeight, JumpHeight;
};

#endif
