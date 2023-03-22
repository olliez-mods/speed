#ifndef GameWindow_h
#define GameWindow_h
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SFAS/Widgets.hpp"
#include "Window.h"
#include "Server.h"
#include "Client.h"

using namespace sfas;

class GameWindow : public Window {
	int newWinId = 9;

	Button nextCardButton;

	sf::Texture tableTexture;
	sf::RectangleShape tableRect;

	WidgetManager widgetManager;

	Button playButton;
public:
	//GameWindow();
	void startWindow();
	void tick(sf::RenderWindow& window);
	int getNewWinId();
};

#endif
