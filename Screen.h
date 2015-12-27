#ifndef SCREEN_H
#define SCREEN_H
#include "Choice.h"
#include "ScreenCode.h"
#include "ClientSFML.h"
#include "ServerSFML.h"
#include <iostream>
#include <boost/thread.hpp>

class Screen
{
public:
    Screen(sf::RenderWindow & window);
    ScreenCode Run(sf::RenderWindow & window);
    virtual void SetupChoices() {};
    virtual void DrawScreen(sf::RenderWindow & window) {}
    virtual void HandleEvent(sf::Event & aEvent) {}
    virtual void HandleOnScreenChoice(Choice c) {}
    virtual void HandleChoice(Choice c) {}
    virtual void HandleClient() {}

    ScreenCode CurrentScreenScreenCode;
    bool operator ==(const Screen &obj) const
    {
        return (obj.CurrentScreenScreenCode == CurrentScreenScreenCode);
    }
    enum ServerClientCommand
    {
        Nothing,
        StartServer,
        StartClient,
        CloseServer,
        CloseClient
    };
    static ServerClientCommand Command;
	static sf::Texture Screen::SpriteSheet;
protected:
    static ServerSFML server;
    static ClientSFML localClient;
    std::vector<Choice>Choices;
    std::vector<sf::FloatRect>ClickableAreas;
    void SetupClickableAreas();
    void DrawChoices(sf::RenderWindow & window);
    int MouseOverButton;
    sf::RenderWindow & Window;
    ScreenCode ResultScreenCode = CurrentScreenScreenCode;
};
#endif
