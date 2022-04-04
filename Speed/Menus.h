#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Widgets.h"
#include "Window.h"
#include "Server.h"
#include "Client.h"

class MainMenu : public Window {
	int newWinId = 1;

	sf::Texture logoTexture;
	sf::RectangleShape logoRect;

	sf::Texture tableTexture;
	sf::RectangleShape tableRect;

	WidgetManager widgetManager;

	Button playButton;
	//Button quitButton;
public:
	void startWindow();
	void tick(sf::RenderWindow& window);
	int getNewWinId();
};

class PlayMenu : public Window {
	int newWinId = 2;

	sf::Texture tableTexture;
	sf::RectangleShape tableRect;

	WidgetManager widgetManager;

	Button joinButton;
	Button hostButton;
public:
	void startWindow();
	void tick(sf::RenderWindow& window);
	int getNewWinId();
};



class JoinMenu : public Window {
	int newWinId = 3;


	sf::Texture tableTexture;
	sf::RectangleShape tableRect;

	sf::UdpSocket UdpSocket;
	sf::IpAddress sender;
	unsigned short port;


	std::vector<Button> LANserverButtons;
	std::vector<std::string> LANserverIps;

	bool listenForBroadcasts = true;

	WidgetManager widgetManager;

	Button joinButton;
	Input inputTest;
public:
	void startWindow();
	void tick(sf::RenderWindow& window);
	void checkEvents(sf::Event event);
	int getNewWinId();
};


class ServerMenu : public Window {
	int newWinId = 4;

	sf::Texture tableTexture;
	sf::RectangleShape tableRect;

	WidgetManager widgetManager;

	Button startServerButton;
	Button localWifiButton;
	Input nameInput;
public:
	void startWindow();
	void tick(sf::RenderWindow& window);
	void checkEvents(sf::Event event);
	int getNewWinId();
};