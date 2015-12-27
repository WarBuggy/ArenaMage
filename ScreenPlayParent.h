#ifndef SCREENPLAYPARENT_H
#define SCREENPLAYPARENT_H

#include "Screen.h"

class ScreenPlayParent : public Screen
{
public:
    ScreenPlayParent(sf::RenderWindow & window);
    void DrawScreen(sf::RenderWindow & window);
private:
    void SetupBoundsVertices();
    void SetupArenaObjectVertices();
    float scale;
    float windowWidth;
    float windowHeight;
    float arenaWidth;
    float arenaHeight;
    float unplayableHeight;
    float unplayableWidth;
	sf::VertexArray arenaVertices;
};

#endif