#include "Widgets.h"
//================
//    DEFAULT
//================
void Widget::display(sf::RenderWindow& window){}
void Widget::tick(sf::Window& window){}
void Widget::setPosistion(float x, float y){}
void Widget::checkEvent(sf::Event event) {}

//================
//    WIDGET manager CLASS
//================

WidgetManager::WidgetManager()
{
	 widgetStack = std::vector<Widget*>();
}

void WidgetManager::registerWidget(Widget* widget){
	widgetStack.push_back(widget);
}

void WidgetManager::clearStack() {
	widgetStack.clear();
}

void WidgetManager::tickStack(sf::RenderWindow& window) {
	for (Widget* widget : widgetStack)
	{
		widget->tick(window);
	}
}

void WidgetManager::renderStack(sf::RenderWindow& window) {
	for (Widget* widget : widgetStack)
	{
		widget->display(window);
	}
}

void WidgetManager::checkEventForStack(sf::Event event)
{
	for (Widget* widget : widgetStack)
	{
		widget->checkEvent(event);
	}
}

//================
//    BUTTON
//================

Button::Button(float x, float y, float width, float height)
{
	rect = sf::RectangleShape(sf::Vector2f(width, height));
	setPosistion(x, y);
}

Button::Button(){}

void Button::tick(sf::Window& window)
{
	sf::Mouse mouse;
	sf::Vector2i mousePos = mouse.getPosition(window);

	mouseOver = (mousePos.x > x && mousePos.x < x + rect.getSize().x &&
				 mousePos.y > y && mousePos.y < y + rect.getSize().y);

	bool mouseDown = mouse.isButtonPressed(mouse.Left);

	isHeld = (mouseOver && mouseDown);

	if (usingtextures)
	{
		if (isHeld)
			rect.setTexture(&pressedTexture);
		else if (mouseDownLastTick && !mouseDown)
			rect.setTexture(&buttonTexture);
	}

	clickedOnTick = (isHeld && !mouseDownLastTick);

	mouseDownLastTick = mouseDown;
}

void Button::setColor(sf::Color color)
{
	rect.setFillColor(color);
}

void Button::setTexture(std::string defaultTextureStr, std::string pressedTextureStr)
{
	if (pressedTextureStr == "-1a")
		pressedTextureStr = defaultTextureStr;

	buttonTexture.loadFromFile(defaultTextureStr);
	pressedTexture.loadFromFile(pressedTextureStr);
	usingtextures = true;
	rect.setTexture(&buttonTexture);
}

void Button::setTexture(sf::Texture defaultTexture)
{
	usingtextures = true;
	buttonTexture = defaultTexture;
	this->pressedTexture = defaultTexture;
	rect.setTexture(&buttonTexture);
}

void Button::setTexture(sf::Texture defaultTexture, sf::Texture pressedTexture)
{
	usingtextures = true;
	buttonTexture = defaultTexture;
	this->pressedTexture = pressedTexture;
	rect.setTexture(&buttonTexture);
}

void Button::setText(std::string text, sf::Color color, int textSize)
{
	font.loadFromFile("arial.ttf");
	rectString.setFont(font);
	rectString.setString(text);
	rectString.setCharacterSize(textSize);
	rectString.setFillColor(color);
	rectString.setPosition(rect.getPosition().x + rect.getSize().x/2-rectString.getLocalBounds().width/2, 
	rect.getPosition().y + rect.getSize().y / 2 - rectString.getLocalBounds().height / 2);
}

std::string Button::getText()
{
	return rectString.getString();
}

void Button::setPosistion(float aX, float aY)
{
	x = aX;
	y = aY;
	rect.setPosition(x, y);
}

void Button::display(sf::RenderWindow& window)
{
	rectString.setFont(font);
	window.draw(rect);
	window.draw(rectString);
}

void Button::checkEvent(sf::Event event)
{

}

//================
//    SLIDER
//================

Slider::Slider(float x, float y, float width, float height, int min, int max)
{
	this->min = min;
	this->max = max;



	baseRect = sf::RectangleShape(sf::Vector2f(width, height));
	sliderRect = sf::RectangleShape(sf::Vector2f(5, height));
	setPosistion(x, y);
}

Slider::Slider() {
	min = 0;
	max = 1;
}

void Slider::doShowValue(bool show, sf::Color color, int size)
{
	showValue = show;
	valueString.setFillColor(color);
	valueString.setCharacterSize(size);
	font.loadFromFile("arial.ttf");
}

void Slider::setPosistion(float x, float y)
{
	this->x = x;
	this->y = y;
	sliderRect.setPosition(x + (sliderRect.getPosition().x - baseRect.getPosition().x), y);
	baseRect.setPosition(x, y);
	valueString.setPosition(baseRect.getPosition().x + baseRect.getSize().x / 2 - valueString.getLocalBounds().width / 2,
	baseRect.getPosition().y + baseRect.getSize().y / 2 - valueString.getLocalBounds().height / 2);
}

void Slider::setColor(sf::Color baseColor, sf::Color sliderColor)
{
	baseRect.setFillColor(baseColor);
	sliderRect.setFillColor(sliderColor);
}

void Slider::setTexture(std::string baseTextureStr, std::string sliderTextureStr)
{
	baseTexture.loadFromFile(baseTextureStr);
	sliderTexture.loadFromFile(sliderTextureStr);
	baseRect.setTexture(&baseTexture);
	sliderRect.setTexture(&sliderTexture);
}

void Slider::setTexture(sf::Texture baseTexture, sf::Texture sliderTexture)
{
	this->baseTexture = baseTexture;
	this->sliderTexture = sliderTexture;

	baseRect.setTexture(&baseTexture);
	sliderRect.setTexture(&sliderTexture);
}

void Slider::tick(sf::Window& window)
{
	sf::Mouse mouse;
	sf::Vector2i mousePos = mouse.getPosition(window);

	mouseOver = (mousePos.x > x && mousePos.x < x + baseRect.getSize().x &&
				 mousePos.y > y && mousePos.y < y + baseRect.getSize().y);

	isHeld = (mouseOver && mouse.isButtonPressed(mouse.Left));

	if (isHeld) 
	{
		sliderRect.setPosition(mousePos.x, sliderRect.getPosition().y);

		float perc = ((baseRect.getSize().x - (sliderRect.getPosition().x - baseRect.getPosition().x)) / baseRect.getSize().x);
		value = perc * (min - max) + max;
	}

	std::string newValueString = (showValueAsInt) ? std::to_string((int)value) : std::to_string(value);
	valueString.setString(newValueString);
	valueString.setPosition(baseRect.getPosition().x + baseRect.getSize().x / 2 - valueString.getLocalBounds().width / 2,
	baseRect.getPosition().y + baseRect.getSize().y / 2 - valueString.getLocalBounds().height / 2);
}

float Slider::getValue()
{
	return value;
}

void Slider::setValue(float newValue)
{
	value = newValue;
	sliderRect.setPosition(baseRect.getPosition().x + ((newValue-min)/(max-min)*baseRect.getSize().x), sliderRect.getPosition().y);
}

void Slider::display(sf::RenderWindow& window)
{
	window.draw(baseRect);
	if (showValue)
	{
		valueString.setFont(font);
		window.draw(valueString);
	}
	window.draw(sliderRect);
}

void Slider::checkEvent(sf::Event event)
{

}

//================
//    SWITCH
//================

Switch::Switch(float x, float y, float width, float height)
{
	baseRect = sf::RectangleShape(sf::Vector2f(width, height));
	baseRect.setFillColor(sf::Color::Red);

	setPosistion(x, y);
	setValue(false);
}

Switch::Switch() {}

bool Switch::getValue()
{
	return value;
}

void Switch::setValue(bool newValue)
{
	value = newValue;
	if (usingTextures) {
		if (newValue)
			baseRect.setTexture(&onTexture);
		else
			baseRect.setTexture(&offTexture);
	}
	else {
		if (!newValue)
			baseRect.setFillColor(sf::Color::Red);
		else
			baseRect.setFillColor(sf::Color::Green);
	}
}

void Switch::setTexture(std::string offTextureStr, std::string onTextureStr)
{
	offTexture.loadFromFile(offTextureStr);
	onTexture.loadFromFile(onTextureStr);

	baseRect.setFillColor(sf::Color::White);
	usingTextures = true;
	setValue(value);
}
void Switch::setTexture(sf::Texture offTexture, sf::Texture onTexture)
{
	this->offTexture = offTexture;
	this->onTexture = onTexture;

	baseRect.setTexture(&offTexture);
	baseRect.setFillColor(sf::Color::White);
	usingTextures = true;
}

void Switch::tick(sf::Window& window)
{
	sf::Mouse mouse;
	sf::Vector2i mousePos = mouse.getPosition(window);

	mouseOver = (mousePos.x > x && mousePos.x < x + baseRect.getSize().x &&
		mousePos.y > y && mousePos.y < y + baseRect.getSize().y);

	isHeld = (mouseOver && mouse.isButtonPressed(mouse.Left));

	clickedOnTick = (isHeld && !mouseDownLastTick);

	if (clickedOnTick)
		setValue(!value);

	mouseDownLastTick = mouse.isButtonPressed(mouse.Left);
}


void Switch::setPosistion(float aX, float aY)
{
	x = aX;
	y = aY;
	baseRect.setPosition(x, y);
	setValue(value);
}

void Switch::display(sf::RenderWindow& window)
{
	window.draw(baseRect);
}

void Switch::checkEvent(sf::Event event)
{

}

//================
//    INPUT
//================



Input::Input(float x, float y, float width, float height, sf::Color color)
{
	font.loadFromFile("arial.ttf");
	stringText.setFont(font);
	stringText.setFillColor(sf::Color::Black);

	rect = sf::RectangleShape(sf::Vector2f(width, height));
	rect.setFillColor(color);
	setPosistion(x, y);
}

Input::Input() {}

void Input::tick(sf::Window& window)
{
	sf::Mouse mouse;
	sf::Vector2i mousePos = mouse.getPosition(window);

	mouseOver = (mousePos.x > x && mousePos.x < x + rect.getSize().x &&
		mousePos.y > y && mousePos.y < y + rect.getSize().y);

	clickedOnTick = (mouseOver && mouse.isButtonPressed(mouse.Left) && !mouseDownLastTick);

	if (clickedOnTick)
		inFocus = true;

	if (mouse.isButtonPressed(mouse.Left) && !mouseOver)
		inFocus = false;

	if (inFocus) {
		while (keysPressedThisTick.size() > 0)
		{
			int key = keysPressedThisTick[0];
			char keyChar = static_cast<char>(key);
			if (key == 8) {
				value = value.substr(0, value.size() - 1);
				setText(value);
			}
			else {
				value += keyChar;
				setText(value);
			}
				keysPressedThisTick.erase(keysPressedThisTick.begin(), keysPressedThisTick.begin() + 1);
		}
	}


	mouseDownLastTick = mouse.isButtonPressed(mouse.Left);
}

void Input::setTextSize(int textSize)
{
	stringText.setCharacterSize(textSize);
}

std::string Input::getText()
{
	return value;
}

void Input::setText(std::string text)
{
	stringText.setString(text);
	stringText.setPosition(rect.getPosition().x,
	rect.getPosition().y + rect.getSize().y / 2 - stringText.getGlobalBounds().height);
}

void Input::setTexture(sf::Texture texture)
{
	rectTexture = texture;
	rect.setTexture(&rectTexture);
}
void Input::setTexture(std::string textureStr)
{
	rectTexture.loadFromFile(textureStr);
	rect.setTexture(&rectTexture);
}

void Input::setPosistion(float aX, float aY)
{
	x = aX;
	y = aY;
	rect.setPosition(x, y);
}

void Input::display(sf::RenderWindow& window)
{
	stringText.setFont(font);
	window.draw(rect);
	window.draw(stringText);
}

void Input::checkEvent(sf::Event event)
{
	keysPressedThisTick.clear();
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128)
			keysPressedThisTick.push_back(event.text.unicode);
	}
}
