#include "ScreenSelect.h"

ScreenSelect::ScreenSelect(sf::RenderWindow & window) : ScreenPlayParent(window)
{
    CurrentScreenScreenCode = ScreenCode::SelectCode;
    ResultScreenCode = CurrentScreenScreenCode;
}
