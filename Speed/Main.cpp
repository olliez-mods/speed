#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include<vector>

#include "GameWindow.h"
#include "Menus.h"
#include "Window.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Speed" , sf::Style::Titlebar | sf::Style::Close);


    MainMenu mainMenuWindow;
    PlayMenu playMenuWindow;
    JoinMenu joinMenuWindow;
    ServerMenu serverMenuWindow;
    GameWindow gameWindow;



    int openWindowId = 1;
    bool switchWindow = true;

    Window* activeWindow = &mainMenuWindow;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            activeWindow->checkEvents(event);
        }

        if (switchWindow)
        {
            switchWindow = false;
            openWindowId = activeWindow->getNewWinId();
            switch (openWindowId)
            {
            case 1:
                activeWindow =&mainMenuWindow;
                break;
            case 2:
                activeWindow = &playMenuWindow;
                break;
            case 3:
                activeWindow = &joinMenuWindow;
                break;
            case 4:
                activeWindow = &serverMenuWindow;
                break;
            case 9:
                activeWindow = &gameWindow;
            }
            activeWindow->startWindow();
        }

        activeWindow->tick(window);
        if (activeWindow->getNewWinId() != openWindowId)
            switchWindow = true;

    }

    return 0;
}