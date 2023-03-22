#include "ColorPicker.hpp"
using namespace sfas;

ColorPicker::ColorPicker(sf::Vector2f pos, sf::Vector2f size, float buffer, bool useAlpha)
{
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    this->buffer = buffer;
    this->useAlpha = useAlpha;
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    
    colorRect = sf::RectangleShape(sf::Vector2f(width - buffer*2, height/7*3 - buffer*2));
    colorRect.setFillColor(sf::Color::Black);

    blackRect = sf::RectangleShape(sf::Vector2f(width - buffer*2, height/7*3 - buffer*2));
    blackRect.setFillColor(sf::Color::Black);
    
    sliderR = Slider(sf::Vector2f(x + buffer, y + height/7*3), sf::Vector2f(width - buffer*2, height/7-buffer), 0, 255);
    sliderG = Slider(sf::Vector2f(x + buffer, y + height/7*4), sf::Vector2f(width - buffer*2, height/7-buffer), 0, 255);
    sliderB = Slider(sf::Vector2f(x + buffer, y + height/7*5), sf::Vector2f(width - buffer*2, height/7-buffer), 0, 255);
    if(useAlpha)
        sliderA = Slider(sf::Vector2f(x + buffer, y + height/7*6), sf::Vector2f(width - buffer*2, height/7-buffer), 0, 255);
    
    sliderR.doShowValue(true);
    sliderG.doShowValue(true);
    sliderB.doShowValue(true);
    if(useAlpha)
        sliderA.doShowValue(true);
    
    sliderR.setColor(sf::Color::White, sf::Color(40, 40, 40));
    sliderG.setColor(sf::Color::White, sf::Color(40, 40, 40));
    sliderB.setColor(sf::Color::White, sf::Color(40, 40, 40));
    if(useAlpha){
        sliderA.setColor(sf::Color::White, sf::Color(40, 40, 40));
        sliderA.setValue(254);
    }
    
    setPosition(pos);
}

ColorPicker::ColorPicker(){}

void ColorPicker::tick(sf::Window& window)
{
    if(enabled)
    {
        sf::Mouse mouse;
        sf::Vector2i mousePos = mouse.getPosition(window);
        
        mouseOver = (mousePos.x > x && mousePos.x < x + rect.getSize().x &&
                     mousePos.y > y && mousePos.y < y + rect.getSize().y);

        bool mouseDown = mouse.isButtonPressed(mouse.Left);
        
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
        
        clickedOnTick = (isHeld && !mouseDownLastTick);
        
        
        sliderR.tick(window);
        sliderG.tick(window);
        sliderB.tick(window);
        if(useAlpha)
            sliderA.tick(window);
        
        colorRect.setFillColor(sf::Color(sliderR.getValue(), sliderG.getValue(), sliderB.getValue(), sliderA.getValue()));
        
        
        mouseDownLastTick = mouseDown;
    }
}

sf::Color ColorPicker::getColor()
{
    return colorRect.getFillColor();
}

void ColorPicker::overrideColor(sf::Color color)
{
    sliderR.setValue(color.r);
    sliderG.setValue(color.g);
    sliderB.setValue(color.b);
    if(useAlpha)
        sliderA.setValue(color.a);
}



void ColorPicker::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void ColorPicker::setTexture(std::string defaultTextureStr)
{
    
    texture.loadFromFile(defaultTextureStr);
    usingTextures = true;
    rect.setTexture(&texture);
}

void ColorPicker::setTexture(sf::Texture defaultTexture)
{
    usingTextures = true;
    texture = defaultTexture;
    rect.setTexture(&texture);
}

void ColorPicker::setPosition(sf::Vector2f pos)
{
    x = pos.x;
    y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
    blackRect.setPosition(sf::Vector2f(pos.x+buffer, pos.y+buffer));
    colorRect.setPosition(sf::Vector2f(pos.x+buffer, pos.y+buffer));
    
    sliderR.setPosition(sf::Vector2f(x + buffer, y + height/7*3));
    sliderG.setPosition(sf::Vector2f(x + buffer, y + height/7*4));
    sliderB.setPosition(sf::Vector2f(x + buffer, y + height/7*5));
    if(useAlpha)
        sliderA.setPosition(sf::Vector2f(x + buffer, y + height/7*6));
}

void ColorPicker::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(rect);
        window.draw(blackRect);
        window.draw(colorRect);
        sliderR.display(window);
        sliderG.display(window);
        sliderB.display(window);
        if(useAlpha)
            sliderA.display(window);
    }
}

void ColorPicker::checkEvent(sf::Event event)
{
    
}


