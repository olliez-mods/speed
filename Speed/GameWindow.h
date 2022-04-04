#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Widgets.h"
#include "Window.h"
#include "Server.h"
#include "Client.h"


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