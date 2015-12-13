#include "ArenaMage.h"

std::vector<ScreenResolution> ArenaMage::SupportedScreenResolutions(2);
sf::Font Choice::DEFAULT_FONT;
size_t Object::SCALE;

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

void SetupSupportedScreenResolution()
{
    ScreenResolution DEFAULT = { 1024, 768 };
    ScreenResolution _800_600 = { 800, 600 };
    ArenaMage::SupportedScreenResolutions.resize(2);
    ArenaMage::SupportedScreenResolutions[0] = DEFAULT; // default resolution is the first
    ArenaMage::SupportedScreenResolutions[1] = _800_600;
}

void SetupFont()
{
    if (!Choice::DEFAULT_FONT.loadFromFile("Resources/arial.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
        getchar();
        exit(-1);
    }
}

void init()
{
    SetupSupportedScreenResolution();
    SetupFont();
}

ScreenCode HandleScreenCode(ScreenCode code, sf::RenderWindow & window)
{
    if (code == ScreenCode::ScreenMenuCode)
    {
        ScreenMenu screen(window);
        return screen.Run(window);
    }
    else if (code == ScreenCode::ConnectServerCode)
    {
        ScreenConnectServer screen(window);
        return screen.Run(window);
    }
    else if (code == ScreenCode::SelectCode)
    {
        ScreenSelect screen(window);
        return screen.Run(window);
    }
    else
    {
        return ScreenCode::Exit;
    }
}

void StartGame()
{
    Object::SCALE = 5;
    //std::map<ScreenCode, Screen*> Screens;
    ScreenCode screencode = ScreenCode::ScreenMenuCode;
    init();
    sf::RenderWindow Window(sf::VideoMode(ArenaMage::SupportedScreenResolutions[0].Width, ArenaMage::SupportedScreenResolutions[0].Height, 32), "Arena Mage");
    //Screens preparations
    //Screens[ScreenCode::ScreenMenuCode] = new ScreenMenu(Window);
    while (screencode != ScreenCode::Exit)
    {
        screencode = HandleScreenCode(screencode, Window);
    }
}

int main()
{
    srand((size_t)time(0));
    try
    {
        StartGame();
        
        //dostay();

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << "Error in starting up server:" << std::endl;
        std::cout << e.what() << std::endl;
    }
    return 0;
}
