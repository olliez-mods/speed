#include "Client.h"

sf::TcpSocket socket;
sf::Packet data;
bool Client::connectToServer(std::string ip)
{
	socket.setBlocking(true);
	sf::IpAddress address = sf::IpAddress::Any;
	try {
		address = sf::IpAddress::resolve(ip).value();
	}
	catch (...) {
		std::cout << "Error, probably invalid IP\n";
		return false;
	}

	std::cout << address.toString() << "\n";

	sf::Socket::Status status = socket.connect(address, 5300, sf::seconds(5));
	socket.setBlocking(false);
	return(status == sf::Socket::Status::Done);
}

void Client::sendData(sf::Packet data)
{
	sf::Packet pack;
	int16_t x = 12;
	pack << x;
	socket.send(data);
}

bool Client::checkNewData()
{
	sf::Packet newData;
	if (socket.receive(newData) != sf::Socket::Status::NotReady)
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
