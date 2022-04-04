#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

#include <chrono>
#include <string>
//#include <algorithm>  
//#include <cstdlib>
#include<random>
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

