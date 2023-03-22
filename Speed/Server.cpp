#include "Server.h"

sf::UdpSocket localUDPSocket;

sf::TcpListener listener;
sf::TcpSocket players[2];

bool plyer1ReadyNext = false;
bool plyer2ReadyNext = false;

float sendPingTimer = 0;

float playerTimeout[2] = { 0, 0 };

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine rng(seed);

sf::Clock dClock;

std::vector<std::vector<int>> deck;
std::vector<std::vector<int>> shuffledDeck;

bool Server::plyer1Connected = false;
bool Server::plyer2Connected = false;
bool Server::runningServer = false;

std::string Server::LANname = "Default";

int gerRnd()
{
    return 10;
}

void Server::startServer()
{
    //localUDPSocket.bind(9400);

    runningServer = true;
    listener.listen(5300);
    listener.setBlocking(false);
    players[0].setBlocking(false);
    players[1].setBlocking(false);

    for (int suit = 0; suit < 4; suit++)
    {
        for (int card = 1; card < 13 + 1; card++)
        {
            deck.push_back(std::vector<int> {suit, card});
        }
    }

}

void sendLocalPing()
{
    sf::Packet data;
    data << "LANserverUp" << Server::LANname << sf::IpAddress::getLocalAddress().value().toString();
    // if this doesnt work use 192.168.1.255 as the broadcast value
    localUDPSocket.send(data, sf::IpAddress::Broadcast, 9400);
    localUDPSocket.send(data, sf::IpAddress::Broadcast, 9500);
    localUDPSocket.send(data, sf::IpAddress::Broadcast, 9600);
}

void Server::tickServer()
{
    sf::Time dt = dClock.restart();

    sendPingTimer += dt.asSeconds();
    if (sendPingTimer >= 1.5 && (!plyer1Connected || !plyer2Connected))
    {
        sendPingTimer = 0;
        sendLocalPing();
    }

    if (plyer1Connected) {
        playerTimeout[0] += dt.asSeconds();
        if (playerTimeout[0] > 3) {
            plyer1Connected = false;
            playerTimeout[0] = 0;
            std::cout << "player 1 disconnected\n";
        }
    }
    else
    {
        if (listener.accept(players[0]) != sf::Socket::Status::NotReady) {
            std::cout << "SERVER : player 1 connected\n";
            plyer1Connected = true;
        }
    }


    if (plyer2Connected) {
        playerTimeout[1] += dt.asSeconds();
        if (playerTimeout[1] > 3) {
            plyer2Connected = false;
            playerTimeout[1] = 0;
            std::cout << "player 2 disconnected\n";
        }
    }
    else
    {
        if (listener.accept(players[1]) != sf::Socket::Status::NotReady) {
            std::cout << "SERVER : player 2 connected\n";
            plyer2Connected = true;
        }
    }


    if (plyer1ReadyNext && plyer2ReadyNext)
    {
        

        int16_t suit;
        int16_t card;
        if (shuffledDeck.size() < 2) {
            shuffledDeck = deck;
            std::shuffle(shuffledDeck.begin(), shuffledDeck.end(), rng);
        }
        suit = shuffledDeck[0][0];
        card = shuffledDeck[0][1];



        sf::Packet packet;
        packet << "setLeft" << suit << card;
        players[0].send(packet);
        players[1].send(packet);

        packet.clear();

        suit = shuffledDeck[1][0];
        card = shuffledDeck[1][1];
        packet << "setRight" << suit << card;
        players[0].send(packet);
        players[1].send(packet);
        shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 2);
        plyer1ReadyNext = false;
        plyer2ReadyNext = false;
    }


    for (int i = 0; i < 2; i++)
    {
        sf::Packet data;
        if (players[i].receive(data) != sf::Socket::Status::NotReady)
        {
            std::string command;
            data >> command;

            if (command == "check")
            {
                playerTimeout[i] = 0;
            }
            if (plyer1Connected && plyer2Connected) {
                if (command == "shuffle")
                {
                    shuffledDeck = deck;
                    std::shuffle(deck.begin(), deck.end(), rng);
                }
                if (command == "win")
                {
                    sf::Packet packet;
                    packet << "win";
                    players[i].send(packet);
                    packet.clear();
                    packet << "lose";
                    if (i == 0)
                        players[1].send(packet);
                    else
                        players[0].send(packet);
                }
                if (command == "nextCard")
                {
                    if (i == 0)
                        plyer1ReadyNext = true;
                    if (i == 1)
                        plyer2ReadyNext = true;
                }
                if (command == "setLeft")
                {
                    players[0].send(data);
                    players[1].send(data);
                }
                if (command == "setRight")
                {
                    players[0].send(data);
                    players[1].send(data);
                }
                if (command == "start")
                {
                    shuffledDeck = deck;

                    std::shuffle(shuffledDeck.begin(), shuffledDeck.end(), rng);
                    for (int p = 0; p < 2; p++)
                    {
                        sf::Packet Qpacket;
                        Qpacket << "newGame";
                        players[p].send(Qpacket);
                        for (int i = 0; i < 20; i++)
                        {
                            int16_t suit = shuffledDeck[i][0];
                            int16_t card = shuffledDeck[i][1];

                            sf::Packet packet;
                            packet << "addToDeck" << suit << card;
                            players[p].send(packet);
                        }
                        shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 20);
                    }



                    int16_t suit = shuffledDeck[0][0];
                    int16_t card = shuffledDeck[0][1];

                    sf::Packet packet;
                    packet << "setLeft" << suit << card;
                    players[0].send(packet);
                    players[1].send(packet);

                    packet.clear();

                    suit = shuffledDeck[1][0];
                    card = shuffledDeck[1][1];
                    packet << "setRight" << suit << card;
                    players[0].send(packet);
                    players[1].send(packet);

                    shuffledDeck.erase(shuffledDeck.begin(), shuffledDeck.begin() + 2);
                }
            }
        }
    }
}

void Server::sendData(sf::Packet data, int toWho)
{
    switch (toWho)
    {
    case 0:
        players[0].send(data);
        players[1].send(data);
        break;
    case 1:
        players[0].send(data);
        break;
    case 2:
        players[1].send(data);
        break;
    }
}
