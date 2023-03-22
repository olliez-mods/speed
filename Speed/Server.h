#ifndef Server_h
#define Server_h
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include<random>
#include <chrono>
class Server
{

	static bool plyer1Connected;
	static bool plyer2Connected;
public:
	static std::string LANname;
	static bool runningServer;
	static void startServer();
	static void tickServer();
	static void sendData(sf::Packet data, int toWho = 0);
};

#endif
