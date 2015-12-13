#ifndef SCREENCONNECTSERVER_H
#define SCREENCONNECTSERVER_H

#include "Screen.h"

class ScreenConnectServer : public Screen
{
public:
    ScreenConnectServer(sf::RenderWindow & window);
    void SetupChoices();
    void DrawScreen(sf::RenderWindow & window);
    void HandleEvent(sf::Event & aEvent);
    void HandleOnScreenChoice(Choice c);
    void HandleChoice(Choice c);
    void HandleClient();
private:
    Choice BackToMainMenu;
};



#endif