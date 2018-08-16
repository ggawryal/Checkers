#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class Button : public sf::Drawable{
    sf::RectangleShape shape;
    sf::Text text;
    void fixTextAlignment(){
        text.setOrigin(text.getLocalBounds().width/2.0f,text.getLocalBounds().height/2.0f);
        sf::Vector2f position = shape.getPosition();
        text.setPosition(sf::Vector2f(position.x + shape.getLocalBounds().width/2.0f,position.y + shape.getLocalBounds().height/2.0f));
    }
    public:
        Button(sf::IntRect rect) {
            setRect(rect);
            text.setFont(FontManager::instance().get("arial"));
            text.setCharacterSize(32);
            text.setColor(sf::Color(70,70,70));
        }
        Button(sf::IntRect rect,sf::Color color) :Button(rect){
            setBackgroundColor(color);
        }
        Button(sf::IntRect rect,sf::Color color, sf::String name) :Button(rect,color){
            setText(name);
        }
        void setPosition(sf::Vector2f position){
            shape.setPosition(position);
            fixTextAlignment();
        }
        sf::Vector2f getPosition() const{
            return shape.getPosition();
        }

        void setSize(sf::Vector2f size){
            shape.setSize((sf::Vector2f)size);
        }
        sf::Vector2f getSize() const{
            return shape.getSize();
        }

        void setRect(sf::IntRect rect){
            setPosition(sf::Vector2f(rect.left,rect.top));
            setSize(sf::Vector2f(rect.width,rect.height));
        }

        void setBackgroundColor(sf::Color color){
            shape.setFillColor(color);
        }
        void setText(sf::String str){
            text.setString(str);
            fixTextAlignment();

        }
        virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override{
            target.draw(shape,states);
            target.draw(text,states);
        }

        bool isPointInside(sf::Vector2i point){
            return (point.x >= shape.getPosition().x && point.y >= shape.getPosition().y &&
               point.x <= shape.getPosition().x + shape.getSize().x && point.y <= shape.getPosition().y + shape.getSize().y);
        }

        bool isClicked(ResizableRenderWindow& window){
            return MouseHandler::instance().getButton() == sf::Mouse::Left &&
                isPointInside(window.mapPixelToStd(MouseHandler::instance().getCurrentMousePosition()));
        }


};

#endif // BUTTON_H
