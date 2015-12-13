#ifndef CHOICE_H
#define CHOICE_H

#include "Point2D.h"
#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include "ScreenResolution.h"

class Choice
{
public:
    static const sf::Color HIGHLIGHT_COLOR;
    static const sf::Color DEFAULT_TEXT_COLOR;
    static const  size_t DEFAULT_FONT_SIZE = 35;
    static sf::Font DEFAULT_FONT;

    std::string Label;
    std::string Description;
    Point2D Pos;
    sf::Color Color;
    bool IsEnable = true;
    bool IsOnScreen = false;
    sf::Text Text;

    Choice();
    void SetLabel(std::string l);
    void DrawChoice(bool isMouseOver, sf::RenderWindow & window);

    bool operator ==(const Choice& c) const
    {
        if (c.Label.compare(Label) == 0)
        {
            return true;
        }
        return false;
    }
    static void SetChoicePosMiddle(std::vector<Choice> & choices, size_t screenWidth, size_t screenHeight);
};

#endif