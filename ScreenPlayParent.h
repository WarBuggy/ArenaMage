#ifndef SCREENPLAYPARENT_H
#define SCREENPLAYPARENT_H

#include "Screen.h"

class ScreenPlayParent : public Screen
{
public:
    ScreenPlayParent(sf::RenderWindow & window);
    void DrawScreen(sf::RenderWindow & window);
private:
    void DrawBounds();
    void DrawArena();
    float scale;
    float windowWidth;
    float windowHeight;
    float arenaWidth;
    float arenaHeight;
    float unplayableHeight;
    float unplayableWidth;

};

#endif