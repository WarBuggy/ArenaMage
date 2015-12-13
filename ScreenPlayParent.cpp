#include "ScreenPlayParent.h"

ScreenPlayParent::ScreenPlayParent(sf::RenderWindow & window) : Screen(window)
{
    scale = (float)Object::SCALE;
    windowWidth = (float)Window.getSize().x;
    windowHeight = (float)Window.getSize().y;
    arenaWidth = localClient.arena.PlayableWidth*scale;
    arenaHeight = localClient.arena.PlayableHeight*scale;
    unplayableHeight = windowHeight - arenaHeight;
    unplayableWidth = windowWidth - arenaWidth;
}

void ScreenPlayParent::DrawScreen(sf::RenderWindow & window)
{
    DrawBounds();
    DrawArena();
}

void ScreenPlayParent::DrawBounds()
{
    // draw ceiling
    sf::RectangleShape ceiling;
    ceiling.setSize(sf::Vector2f(windowWidth, unplayableHeight / 2));
    ceiling.setFillColor(sf::Color(0, 0, 255));
    ceiling.setPosition(0, 0);
    Window.draw(ceiling);
    // draw ground
    sf::RectangleShape ground;
    ground.setSize(sf::Vector2f(windowWidth, unplayableHeight / 2));
    ground.setFillColor(sf::Color(0, 0, 128));
    ground.setPosition(0, unplayableHeight / 2 + arenaHeight);
    Window.draw(ground);
    // draw left wall
    sf::RectangleShape left;
    left.setSize(sf::Vector2f(unplayableWidth / 2, windowHeight - unplayableHeight));
    left.setFillColor(sf::Color(0, 0, 139));
    left.setPosition(0, (unplayableHeight / 2));
    Window.draw(left);
    // draw right wall
    sf::RectangleShape right;
    right.setSize(sf::Vector2f(unplayableWidth / 2, windowHeight - unplayableHeight));
    right.setFillColor(sf::Color(0, 0, 205));
    right.setPosition((unplayableWidth / 2) + arenaWidth, unplayableHeight / 2);
    Window.draw(right);
}

void ScreenPlayParent::DrawArena()
{
    float scale = (float)Object::SCALE;
    sf::Vector2f offset(unplayableWidth / 2, unplayableHeight / 2);
    for (std::map<Point2D, ObjectType>::iterator it = localClient.arena.ArenaObjects.begin(); it != localClient.arena.ArenaObjects.end(); ++it)
    {
        sf::Vector2f finalPos(it->first.X*scale + offset.x, it->first.Y*scale + offset.y);
        if (it->second == ObjectType::ARENA_PLATFORM)
        {
            sf::RectangleShape platform;
            ArenaPlatform tempt(it->first);
            platform.setSize(sf::Vector2f(tempt.width*scale, (float)tempt.height));
            platform.setFillColor(sf::Color(255, 215, 0));
            platform.setPosition(finalPos.x, finalPos.y);
            Window.draw(platform);
        }
        else if (it->second == ObjectType::ARENA_BLOCK)
        {
            sf::RectangleShape block;
            ArenaBlock tempt(it->first);
            block.setSize(sf::Vector2f(tempt.width*scale, (float)tempt.height));
            block.setFillColor(sf::Color(139, 69, 19));
            block.setPosition(finalPos.x, finalPos.y);
            Window.draw(block);
        }
    }
}
