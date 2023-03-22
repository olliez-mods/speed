#ifndef Client_h
#define Client_h
#include <SFML/Network.hpp>
#include <iostream>
class Client
{
public:
	static bool connectToServer(std::string ip);
	static void sendData(sf::Packet data);
	static bool checkNewData();
	static sf::Packet getData();
};
#endif
