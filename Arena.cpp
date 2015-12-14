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
