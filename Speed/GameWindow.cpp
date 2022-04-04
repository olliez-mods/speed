#include "GameWindow.h"
sf::Texture cardsTexture;
sf::Texture backOfCardTexture;


class Card
{
public:
	sf::Sprite sprite;
	std::vector<int> cardNum;
	bool active = true;

	Card(int suit, int card, bool active = true)
	{
		this->active = active;
		cardNum = std::vector<int> {suit, card};
		sprite = sf::Sprite(cardsTexture, sf::IntRect(73 * (card - 1) + 1, 98 * suit - 1, 70, 94));
	}
	void setCard(int suit, int card, bool active = true)
	{
		this->active = active;
		cardNum = std::vector<int>{ suit, card };
		sprite.setTexture(cardsTexture);
		sprite.setTextureRect(sf::IntRect(73 * (card - 1), 98 * suit, 73, 98));
	}
	void setCard(std::vector<int> card, bool active = true)
	{
		this->active = active;
		cardNum = std::vector<int>{ card[0], card[1] };
		sprite.setTexture(cardsTexture);
		sprite.setTextureRect(sf::IntRect(73 * (card[1] - 1), 98 * card[0], 73, 98));
	}
	void setCard(Card card, bool active = true)
	{
		this->active = active;
		cardNum = card.cardNum;
		sprite.setTexture(cardsTexture);
		sprite.setTextureRect(sf::IntRect(73 * (cardNum[1] - 1), 98 * cardNum[0], 73, 98));
	}
};

sf::Sprite pickupCard;
sf::Sprite flipOverCard;

std::vector<Card> deck;
std::vector<Card> hand;
Card left(0, 0);
Card right(0, 0);

float blipTimer = 0;
sf::Clock deltaClock;

bool mouseDownLastFrame = false;

int selectedCardIndex = -1;



int GameWindow::getNewWinId()
{
	return newWinId;
}

void resetHandPositions()
{

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i].sprite.setScale(sf::Vector2f(2, 2));
		if (hand.size() < 5)
			hand[i].sprite.setPosition(hand[i].sprite.getPosition().x, 500);
		else
			hand[i].sprite.setPosition(i * 100 + 200, 500);
	}
}

void GameWindow::startWindow()
{
	widgetManager = WidgetManager();

	cardsTexture.loadFromFile("cards.png");

	backOfCardTexture.loadFromFile("back.png");

	resetHandPositions();

	flipOverCard.setTexture(backOfCardTexture);
	flipOverCard.setScale(0.15, 0.15);
	flipOverCard.setPosition(600, 200);

	pickupCard.setTexture(backOfCardTexture);
	pickupCard.setScale(0.15, 0.15);
	pickupCard.setPosition(15, 500);

	left.sprite.setPosition(200, 200);
	left.sprite.setScale(sf::Vector2f(2, 2));
	right.sprite.setPosition(420, 200);
	right.sprite.setScale(sf::Vector2f(2, 2));

	playButton = Button(10, 10, 100, 50);
	playButton.setTexture("button.png", "button_pressed.png");
	widgetManager.registerWidget(&playButton);
	playButton.setText("RESTART", sf::Color::Black, 15);

	//nextCardButton = Button(600, 200, 100, 80, sf::Color::Green);
	//widgetManager.registerWidget(&nextCardButton);
	//nextCardButton.setText("NEXT CARD", sf::Color::Black, 15);

	tableTexture = sf::Texture();
	tableTexture.loadFromFile("table.jpg");

	tableRect = sf::RectangleShape(sf::Vector2f(800, 600));
	tableRect.setTexture(&tableTexture);
}

void GameWindow::tick(sf::RenderWindow& window)
{
	//=======
	//  tick
	//=======
	sf::Time dt = deltaClock.restart();
	blipTimer += dt.asSeconds();

	widgetManager.tickStack(window);

	if (Server::runningServer)
		Server::tickServer();

	if (Client::checkNewData())
	{
		std::string command;
		Client::getData() >> command;

		if (command == "addCard"){
			sf::Uint16 suit = 1;
			sf::Uint16 card = 11;
			Client::getData() >> command >> suit >> card;
		}else 
		if (command == "newGame"){
			deck.clear();
			hand.clear();
			hand = {
				Card(0, 1, false),
				Card(0, 1, false),
				Card(0, 1, false),
				Card(0, 1, false),
				Card(0, 1, false),
			};
			std::cout << "new game\n";
		}else
		if (command == "addToDeck") {
			sf::Uint16 suit;
			sf::Uint16 card;
			Client::getData() >> command >> suit >> card;
			deck.push_back(Card(suit, card));
		}else
		if (command == "setLeft") {
			sf::Uint16 suit;
			sf::Uint16 card;
			Client::getData() >> command >> suit >> card;
			left.setCard(suit, card);
		}else
		if (command == "setRight") {
			sf::Uint16 suit;
			sf::Uint16 card;
			Client::getData() >> command >> suit >> card;
			right.setCard(suit, card);
		}
		else
		if (command == "win") {

		}
		else
		if (command == "lose") {

		}
	}

	if (blipTimer > 0.5)
	{
		blipTimer = 0;
		sf::Packet packet;
		packet << "check";
		Client::sendData(packet);
	}

	if (playButton.clickedOnTick)
	{
		sf::Packet packet;
		packet << "start";
		Client::sendData(packet);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseDownLastFrame)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::FloatRect bounds = flipOverCard.getGlobalBounds();
		if (mousePos.x > bounds.left && mousePos.x < bounds.left + bounds.width &&
			mousePos.y > bounds.top && mousePos.y < bounds.top + bounds.height)
		{
			sf::Packet packet;
			packet << "nextCard";
			Client::sendData(packet);
		}
	}

	/*
	if (nextCardButton.clickedOnTick)
	{
		sf::Packet packet;
		packet << "nextCard";
		Client::sendData(packet);
	}
	*/





	if (selectedCardIndex >= 0)
	{
		if (hand[selectedCardIndex].active) {
			bool removeCard = false;
			bool unselectCard = false;
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
			   (pos.x > left.sprite.getGlobalBounds().left && pos.x < left.sprite.getGlobalBounds().left + left.sprite.getGlobalBounds().width &&
				pos.y > left.sprite.getGlobalBounds().top && pos.y < left.sprite.getGlobalBounds().top + left.sprite.getGlobalBounds().height && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseDownLastFrame))
			{
				if (hand[selectedCardIndex].cardNum[1] == left.cardNum[1] - 1 || hand[selectedCardIndex].cardNum[1] == left.cardNum[1] + 1 ||
					(hand[selectedCardIndex].cardNum[1] == 1 && left.cardNum[1] == 13) || (hand[selectedCardIndex].cardNum[1] == 13 && left.cardNum[1] == 1)) {
					sf::Packet packet;
					packet << "setLeft" << sf::Uint16(hand[selectedCardIndex].cardNum[0]) << sf::Uint16(hand[selectedCardIndex].cardNum[1]);
					Client::sendData(packet);
					removeCard = true;
				}
				unselectCard = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
				(pos.x > right.sprite.getGlobalBounds().left && pos.x < right.sprite.getGlobalBounds().left + right.sprite.getGlobalBounds().width &&
				 pos.y > right.sprite.getGlobalBounds().top && pos.y < right.sprite.getGlobalBounds().top + right.sprite.getGlobalBounds().height && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseDownLastFrame))
			{
				if (hand[selectedCardIndex].cardNum[1] == right.cardNum[1] - 1 || hand[selectedCardIndex].cardNum[1] == right.cardNum[1] + 1 ||
					(hand[selectedCardIndex].cardNum[1] == 1 && right.cardNum[1] == 13) || (hand[selectedCardIndex].cardNum[1] == 13 && right.cardNum[1] == 1)) {
					sf::Packet packet;
					packet << "setRight" << sf::Uint16(hand[selectedCardIndex].cardNum[0]) << sf::Uint16(hand[selectedCardIndex].cardNum[1]);
					Client::sendData(packet);
					removeCard = true;
				}
				unselectCard = true;
			}
			if (removeCard) {
				hand[selectedCardIndex].active = false;
				resetHandPositions();
			}
			if (unselectCard) {
				selectedCardIndex = -1;
				resetHandPositions();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseDownLastFrame) {
				selectedCardIndex = -1;
			}
		}
	}

	if (deck.size() == 0 && hand.size() == 0)
	{
		sf::Packet packet;
		packet << "win";
		Client::sendData(packet);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseDownLastFrame) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		resetHandPositions();
		for (int i = hand.size() - 1; i >= 0; i--)
		{
			sf::FloatRect bounds = hand[i].sprite.getGlobalBounds();
			if (pos.x > bounds.left && pos.x < bounds.left + bounds.width &&
				pos.y > bounds.top && pos.y < bounds.top + bounds.height && hand[i].active)
			{
				selectedCardIndex = i;
				hand[i].sprite.setPosition(hand[i].sprite.getPosition() - sf::Vector2f(0, 20));
				break;
			}
		}
	}

	if (deck.size() > 0 && !mouseDownLastFrame && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::FloatRect bounds = pickupCard.getGlobalBounds();
		if (mousePos.x > bounds.left && mousePos.x < bounds.left + bounds.width &&
			mousePos.y > bounds.top && mousePos.y < bounds.top + bounds.height)
		{
			for (Card &card : hand)
			{

				if (!card.active)
				{
					card.setCard(deck[0]);
					card.active = true;
					deck.erase(deck.begin(), deck.begin() + 1);
					break;
				}
			}
			resetHandPositions();
		}
	}


	mouseDownLastFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	window.clear();
	window.draw(tableRect);
	widgetManager.renderStack(window);

	window.draw(left.sprite);
	window.draw(right.sprite);

	for (Card card : hand)
	{
		if(card.active)
		window.draw(card.sprite);
	}
	window.draw(flipOverCard);
	window.draw(pickupCard);

	window.display();
}