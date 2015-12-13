#include "ScreenConnectServer.h"

ScreenConnectServer::ScreenConnectServer(sf::RenderWindow & window) : Screen(window)
{
    CurrentScreenScreenCode = ScreenCode::ConnectServerCode;
    ResultScreenCode = CurrentScreenScreenCode;
    Choices.clear();
    BackToMainMenu.SetLabel("Back to main menu");
    BackToMainMenu.Text.setStyle(sf::Text::Bold);
    Choices.push_back(BackToMainMenu);
    Choice::SetChoicePosMiddle(Choices, Window.getSize().x, Window.getSize().y);
    Command = ServerClientCommand::StartServer;
}

void ScreenConnectServer::SetupChoices()
{
}

void ScreenConnectServer::DrawScreen(sf::RenderWindow & window)
{
}

void ScreenConnectServer::HandleEvent(sf::Event & aEvent)
{
}

void ScreenConnectServer::HandleOnScreenChoice(Choice c)
{
}

void ScreenConnectServer::HandleChoice(Choice c)
{
    if (c == BackToMainMenu)
    {
        ServerSFML::StopServer = true;
        ClientSFML::StopClient = true;
        ResultScreenCode = ScreenCode::ScreenMenuCode;
    }
}

void ScreenConnectServer::HandleClient()
{
    if (localClient.State == ClientSFML::STATE::ArenaInfoCompleted)
    {
        ResultScreenCode = ScreenCode::SelectCode;
    }
}
