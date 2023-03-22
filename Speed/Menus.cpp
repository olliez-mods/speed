#include "Menus.h"
#include "SFAS/RpFinder.hpp"
#include "SFAS/Conversion.hpp"
using namespace sfas;

int MainMenu::getNewWinId()
{
	return newWinId;
}


void MainMenu::startWindow()
{
    Widget::setFont(rp() + "arial.ttf");
	widgetManager = WidgetManager();

	playButton = Button(c(320, 280), c(160, 100));
	playButton.setTexture(rp() + "button.png", rp() + "button_pressed.png");
	widgetManager.registerWidget(&playButton);
	playButton.setText("PLAY", sf::Color::Black, 30);

	//quitButton = Button(10, 80, 80, 50);
	//quitButton.setTexture("button.png", "button_pressed.png");
	//widgetManager.registerWidget(&quitButton);
	//quitButton.setText("QUIT", sf::Color::Black, 15);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile(rp() + "table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);

	logoTexture = sf::Texture();
	logoTexture.loadFromFile(rp() + "logo.png");

	logoRect = sf::RectangleShape(sf::Vector2f(540, 150));
	logoRect.setPosition(c(130, 100));
	logoRect.setTexture(&logoTexture);
}

void MainMenu::tick(sf::RenderWindow& window)
{
	//=======
	//  tick
	//=======
	widgetManager.tickStack(window);


	if (playButton.clickedOnTick)
		newWinId = 2;


	window.clear();
	window.draw(tableRect);
	widgetManager.renderStack(window);
	window.draw(logoRect);
	window.display();
}



int PlayMenu::getNewWinId()
{
	return newWinId;
}


void PlayMenu::startWindow()
{
	widgetManager = WidgetManager();



	joinButton = Button(c(300, 270), c(100, 60));
	joinButton.setTexture(rp() + "button.png", rp() + "button_pressed.png");
	widgetManager.registerWidget(&joinButton);
	joinButton.setText("JOIN", sf::Color::Black, 18);

	hostButton = Button(c(420, 270), c(100, 60));
	hostButton.setTexture(rp() + "button.png", rp() + "button_pressed.png");
	widgetManager.registerWidget(&hostButton);
	hostButton.setText("HOST", sf::Color::Black, 18);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile(rp() + "table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);
}

void PlayMenu::tick(sf::RenderWindow& window)
{
	//=======
	//  tick
	//=======
	widgetManager.tickStack(window);

	if (joinButton.clickedOnTick) {
		newWinId = 3;
	}

	if (hostButton.clickedOnTick) {
		std::cout << "host\n";
		newWinId = 4;
	}

	if (Server::runningServer)
	{
		Server::tickServer();
	}




	window.clear();
	window.draw(tableRect);
	widgetManager.renderStack(window);
	window.display();
}






int JoinMenu::getNewWinId()
{
	return newWinId;
}


void JoinMenu::startWindow()
{
	widgetManager = WidgetManager();

	joinButton = Button(c(200, 60), c(80, 50));
	joinButton.setTexture(rp() + "button.png", rp() + "button_pressed.png");
	widgetManager.registerWidget(&joinButton);
	joinButton.setText("JOIN", sf::Color::Black, 15);

	statusText.setFont(Widget::getFont());
	statusText.setCharacterSize(20);
	statusText.setPosition(c(15, 130));
	statusText.setFillColor(sf::Color::Black);
	statusText.setString("Waiting for IP");

	if (UdpSocket.bind(9400) != sf::Socket::Status::Done)
	{
		std::cout << "First failed\n";
		if (UdpSocket.bind(9500) != sf::Socket::Status::Done)
		{
			std::cout << "Second Failed\n";
			if (UdpSocket.bind(9600) != sf::Socket::Status::Done)
			{
				std::cout << "Couldn't listen for LAN networks\n";
				listenForBroadcasts = false;
			}
		}
	}
	UdpSocket.setBlocking(false);

	inputTest = Input(c(10, 60), c(180, 50));
	inputTest.setTextSize(28);
	widgetManager.registerWidget(&inputTest);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile(rp() + "table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);
}

void JoinMenu::checkEvents(sf::Event event)
{
	widgetManager.checkEventForStack(event);
}

void JoinMenu::tick(sf::RenderWindow& window)
{
	//=======
	//  tick
	//=======

	widgetManager.tickStack(window);
	for (Button& button : LANserverButtons)
	{
		button.tick(window);
	}

	bool attemptJoin = false;
	if (joinButton.clickedOnTick) {
		std::cout << "join\n";
		statusText.setString("Attempting to connect:");
		attemptJoin = true;

	}
	for (int i = 0; i < LANserverButtons.size(); i++)
	{
		if (LANserverButtons[i].clickedOnTick)
		{
			Client::connectToServer(LANserverIps[i]);
			newWinId = 9;
		}
	}

	if(listenForBroadcasts){
		sf::Packet data;
        std::optional<sf::IpAddress> address;
		if (UdpSocket.receive(data, address, port) != sf::Socket::Status::NotReady)
		{
            sender = address.value();
			std::string dataOut;
			std::string name;
			std::string ipAddress;
			data >> dataOut >> name >> ipAddress;
			if (dataOut == "LANserverUp") {
				bool found = false;
				for (Button &button : LANserverButtons)
				{
					if (button.getText() == name)
						found = true;
				}


				if (!found) {
					LANserverButtons.push_back(Button(c(600, LANserverButtons.size() * 60 + 50), c(180, 50)));
					LANserverButtons[LANserverButtons.size() - 1].setTexture(rp() + "button.png", rp() + "button_pressed.png");
					LANserverButtons[LANserverButtons.size() - 1].setText(name, sf::Color::Black, 30);
					//widgetManager.registerWidget(&LANserverButtons[LANserverButtons.size() - 1]);
					LANserverIps.push_back(sender.toString());
				}
			}
		}
	}

	if (Server::runningServer)
	{
		Server::tickServer();
	}


	window.clear();
	window.draw(tableRect);
	widgetManager.renderStack(window);
	for (Button &button : LANserverButtons)
	{
		button.display(window);
	}
	window.draw(statusText);
	window.display();


	if (attemptJoin) {
		bool result = Client::connectToServer(inputTest.getText());
		if (result) {
			statusText.setString("Attempting to connect: Success");
			newWinId = 9;
		}
		else {
			statusText.setString("Attempting to connect: Failed");
		}
	}
}








int ServerMenu::getNewWinId()
{
	return newWinId;
}


void ServerMenu::startWindow()
{
	widgetManager = WidgetManager();

	startServerButton = Button(c(200, 60), c(120, 50));
	startServerButton.setTexture(rp() + "button.png", rp() + "button_pressed.png");
	widgetManager.registerWidget(&startServerButton);
	startServerButton.setText("HOST GAME", sf::Color::Black, 15);

	nameInput = Input(c(10, 60), c(180, 50));
	nameInput.setTextSize(25);
	widgetManager.registerWidget(&nameInput);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile(rp() + "table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);
}

void ServerMenu::checkEvents(sf::Event event)
{
	widgetManager.checkEventForStack(event);
}

void ServerMenu::tick(sf::RenderWindow& window)
{
	//=======
	//  tick
	//=======
	widgetManager.tickStack(window);

	if (startServerButton.clickedOnTick) {
		Server::LANname = nameInput.getText();
		Server::startServer();
		std::cout << Client::connectToServer("127.0.0.1") << "\n";

		newWinId = 9;
	}

	if (Server::runningServer)
	{
		Server::tickServer();
	}

	window.clear();
	window.draw(tableRect);
	widgetManager.renderStack(window);
	window.display();
}
