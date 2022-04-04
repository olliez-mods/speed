#include "Menus.h"


int MainMenu::getNewWinId()
{
	return newWinId;
}


void MainMenu::startWindow()
{
	widgetManager = WidgetManager();

	playButton = Button(320, 280, 160, 100);
	playButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&playButton);
	playButton.setText("PLAY", sf::Color::Black, 30);

	//quitButton = Button(10, 80, 80, 50);
	//quitButton.setTexture("button.png", "button_pressed.png");
	//widgetManager.registerWidget(&quitButton);
	//quitButton.setText("QUIT", sf::Color::Black, 15);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile("table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);

	logoTexture = sf::Texture();
	logoTexture.loadFromFile("logo.png");

	logoRect = sf::RectangleShape(sf::Vector2f(540, 150));
	logoRect.setPosition(130, 100);
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



	joinButton = Button(10, 10, 80, 50);
	joinButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&joinButton);
	joinButton.setText("JOIN", sf::Color::Black, 15);

	hostButton = Button(10, 80, 80, 50);
	hostButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&hostButton);
	hostButton.setText("HOST", sf::Color::Black, 15);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile("table.jpg");

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

	joinButton = Button(200, 60, 80, 50);
	joinButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&joinButton);
	joinButton.setText("JOIN", sf::Color::Black, 15);

	if (UdpSocket.bind(9400) != sf::Socket::Done)
	{
		std::cout << "First failed\n";
		if (UdpSocket.bind(9500) != sf::Socket::Done)
		{
			std::cout << "Second Failed\n";
			if (UdpSocket.bind(9600) != sf::Socket::Done)
			{
				std::cout << "Couldn't listen for LAN networks\n";
				listenForBroadcasts = false;
			}
		}
	}
	UdpSocket.setBlocking(false);

	inputTest = Input(10, 60, 180, 30);
	inputTest.setTextSize(25);
	widgetManager.registerWidget(&inputTest);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile("table.jpg");

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


	if (joinButton.clickedOnTick) {
		std::cout << "join\n";
		Client::connectToServer(inputTest.getText());
		newWinId = 9;
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
		if (UdpSocket.receive(data, sender, port) != sf::Socket::NotReady)
		{

			std::string dataOut;
			std::string name;
			std::string ipAddress;
			data >> dataOut >> name >> ipAddress;
			if (dataOut == "LANserverUp") {
				bool found = false;
				for (Button button : LANserverButtons)
				{
					if (button.getText() == name)
						found = true;
				}


				if (!found) {
					LANserverButtons.push_back(Button(600, LANserverButtons.size() * 60 + 50, 180, 50));
					LANserverButtons[LANserverButtons.size() - 1].setTexture("button.png", "button_pressed.png");
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
	for (Button button : LANserverButtons)
	{
		button.display(window);
	}
	window.display();
}








int ServerMenu::getNewWinId()
{
	return newWinId;
}


void ServerMenu::startWindow()
{
	widgetManager = WidgetManager();

	startServerButton = Button(200, 60, 120, 50);
	startServerButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&startServerButton);
	startServerButton.setText("HOST GAME", sf::Color::Black, 15);

	nameInput = Input(10, 60, 180, 50);
	nameInput.setTextSize(25);
	widgetManager.registerWidget(&nameInput);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile("table.jpg");

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
		Client::connectToServer("127.0.0.1");

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