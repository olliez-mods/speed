#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Widgets.h"

class Window
{
public:
	int newWinId;
	int winId;
	virtual void tick(sf::RenderWindow& window);
	virtual void checkEvents(sf::Event event);
	virtual int getNewWinId();
	virtual void startWindow();
};
