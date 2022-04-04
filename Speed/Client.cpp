#include "Client.h"

sf::TcpSocket socket;
sf::Packet data;
void Client::connectToServer(std::string ip)
{
	sf::Socket::Status status = socket.connect(ip, 5300);
	socket.setBlocking(false);
}

void Client::sendData(sf::Packet data)
{
	sf::Packet pack;
	sf::Uint16 x = 12;
	pack << x;
	socket.send(data);
}

bool Client::checkNewData()
{
	sf::Packet newData;
	if (socket.receive(newData) != sf::Socket::NotReady)
	{
		data = newData;
		return true;
	}
	return false;
}

sf::Packet Client::getData()
{
	return data;
}