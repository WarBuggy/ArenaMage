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
            std::uniform_int_distribution<int> uni(0, 100);
            size_t chance = uni(rng);
            if (chance <= ArenaBlock::Chance)
            {
                std::uniform_int_distribution<int> repeatRNG(ArenaBlock::MinBlockNum, ArenaBlock::MinBlockNum);
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
                std::uniform_int_distribution<int> repeatRNG(ArenaPlatform::MinBlockNum, ArenaPlatform::MinBlockNum);
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

/*
void SetupArenaObjects()
{
    int rows = GameManager.PlayableHeight / (GameManager.JUMP_HEIGHT_IN_BLOCK * GameManager.BLOCK_SIZE);
    int NumberOfHorizontalBlock = GameManager.PlayableWidth / GameManager.BLOCK_SIZE;
    ArenaObjects = new Dictionary<Vector2, ArenaObject>();
    for (int i = 1; i < rows; i++) // i is 1 because the ground has no objects, yet
    {
        for (int j = 0; j < NumberOfHorizontalBlock; j++)
        {
            int chance = GameManager.TheRandom.Next(0, 101);
            if (chance <= ArenaBlock.Chance)
            {
                int repeat = GameManager.TheRandom.Next(ArenaBlock.MinBlockNum, ArenaBlock.MaxBlockNum + 1);
                int k;
                for (k = j; (k < j + repeat) && (k < NumberOfHorizontalBlock); k++)
                {
                    int x = GameManager.THE_LEFTWALL + (k * GameManager.BLOCK_SIZE);
                    int y = GameManager.THE_GROUND - (i * GameManager.JUMP_HEIGHT_IN_BLOCK * GameManager.BLOCK_SIZE);
                    ArenaBlock block = new ArenaBlock(x, y);
                    ArenaObjects.Add(new Vector2(x, y), block);
                }
                j = k - 1;
            }
            else
            {
                int repeat = GameManager.TheRandom.Next(ArenaPlatform.MinBlockNum, ArenaPlatform.MaxBlockNum + 1);
                int k;
                for (k = j; (k < j + repeat) && (k < NumberOfHorizontalBlock); k++)
                {
                    int x = GameManager.THE_LEFTWALL + (k * GameManager.BLOCK_SIZE);
                    int y = GameManager.THE_GROUND - (i * GameManager.JUMP_HEIGHT_IN_BLOCK * GameManager.BLOCK_SIZE);
                    ArenaPlatform platform = new ArenaPlatform(x, y);
                    ArenaObjects.Add(new Vector2(x, y), platform);
                }
                j = k - 1;
            }
        }
    }
}
*/