#pragma once
#include <SFML/Network.hpp>
#include <iostream>
class Client
{
public:
	static void connectToServer(std::string ip);
	static void sendData(sf::Packet data);
	static bool checkNewData();
	static sf::Packet getData();
};