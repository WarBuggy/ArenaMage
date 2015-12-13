#include "Screen.h"
Screen::ServerClientCommand Screen::Command = Screen::ServerClientCommand::Nothing;
ServerSFML Screen::server;
ClientSFML Screen::localClient;

Screen::Screen(sf::RenderWindow & window) : Window(window)
{
}

ScreenCode Screen::Run(sf::RenderWindow & window)
{
    sf::Event Event;
    bool Running = true;
    while (Running)
    {
        //Verifying events
        while (window.pollEvent(Event))
        {
            // Window closed
            if (Event.type == sf::Event::Closed)
            {
                ServerSFML::StopServer = true;
                ClientSFML::StopClient = true;
                return (ScreenCode::Exit);
            }
            sf::Vector2f mousePos((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
            if (ClickableAreas.size() > 0)
            {
                size_t i = 0;
                while (i < ClickableAreas.size())
                {
                    if (ClickableAreas[i].contains(mousePos))
                    {
                        MouseOverButton = i;
                        break;
                    }
                    i++;
                }
                if (i >= ClickableAreas.size())
                {
                    MouseOverButton = -1;
                }

                if (Event.type == sf::Event::MouseButtonPressed)
                {
                    if (Event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (MouseOverButton >= 0 && Choices[MouseOverButton].IsEnable)
                        {
                            if (Choices[MouseOverButton].IsOnScreen)
                            {
                                HandleOnScreenChoice(Choices[MouseOverButton]);
                            }
                            else
                            {
                                HandleChoice(Choices[MouseOverButton]);
                            }
                            break;
                        }
                    }
                }

            }
        }

        window.clear();
        SetupChoices();
        DrawChoices(window);
        SetupClickableAreas();
        DrawScreen(window);
        window.display();

        if (Command == ServerClientCommand::StartServer)
        {
            server = ServerSFML("1111", 8, 2, 7777);
            boost::thread(boost::bind(&ServerSFML::run, boost::ref(server)));
            Command = ServerClientCommand::StartClient;
        }
        else if (Command == ServerClientCommand::StartClient)
        {
            //localClient = ClientSFML("1111", "Buggy", "93.91.0.247", 7777);
            localClient = ClientSFML("1111", "Buggy", "127.0.0.1", 7777);
            boost::thread(boost::bind(&ClientSFML::run, boost::ref(localClient)));
            Command = ServerClientCommand::Nothing;
        }

        HandleClient();

        if (ResultScreenCode != CurrentScreenScreenCode)
        {
            return ResultScreenCode;
        }
    }
    //Never reaching this point normally, but just in case, exit the application
    return (ScreenCode::Exit);
}

void Screen::DrawChoices(sf::RenderWindow & window)
{
    for (std::vector<size_t>::size_type i = 0; i != Choices.size(); i++) {
        bool isMouseOver = (i == MouseOverButton);
        Choices[i].DrawChoice(isMouseOver, window);
    }
}

void Screen::SetupClickableAreas()
{
    ClickableAreas.clear();
    for (std::vector<size_t>::size_type i = 0; i != Choices.size(); i++) {
        sf::FloatRect rec = Choices[i].Text.getGlobalBounds();
        ClickableAreas.push_back(rec);
    }
}
