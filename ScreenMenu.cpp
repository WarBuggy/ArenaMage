#include "ScreenMenu.h"

ScreenMenu::ScreenMenu(sf::RenderWindow & window) : Screen(window)
{
    CurrentScreenScreenCode = ScreenCode::ScreenMenuCode;
    ResultScreenCode = CurrentScreenScreenCode;
    Choices.clear();
    SinglePlay.SetLabel("Singleplay");
    SinglePlay.Text.setStyle(sf::Text::Bold);
    Choices.push_back(SinglePlay);
    Quit.SetLabel("Quit");
    Quit.Text.setStyle(sf::Text::Bold);
    Choices.push_back(Quit);
    Choice::SetChoicePosMiddle(Choices, Window.getSize().x, Window.getSize().y);
}

void ScreenMenu::SetupChoices()
{
}

void ScreenMenu::DrawScreen(sf::RenderWindow & window)
{
}

void ScreenMenu::HandleEvent(sf::Event & event)
{
}

void ScreenMenu::HandleOnScreenChoice(Choice c)
{
}

void ScreenMenu::HandleChoice(Choice c)
{
    if (c == Quit)
    {
        ResultScreenCode = ScreenCode::Exit;
    }
    else if (c == SinglePlay)
    {
        ResultScreenCode = ScreenCode::ConnectServerCode;
    }
}


