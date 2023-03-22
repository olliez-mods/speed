#ifndef Window_h
#define Window_h

#include <iostream>
#include <SFML/Graphics.hpp>
#include "SFAS/Widgets.hpp"

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
#endif
