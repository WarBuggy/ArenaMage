#ifndef SCREENMENU_H
#define SCREENMENU_H

#include "Screen.h"

class ScreenMenu : public Screen
{
public:
    ScreenMenu(sf::RenderWindow & window);
    void SetupChoices();
    void DrawScreen(sf::RenderWindow & window);
    void HandleEvent(sf::Event & aEvent);
    void HandleOnScreenChoice(Choice c);
    void HandleChoice(Choice c) override;
private:
    Choice SinglePlay, Multiplay, Setting, Quit;
};

#endif

