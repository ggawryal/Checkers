#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H
#include <vector>
#include <SFML/Graphics.hpp>

class MouseHandler
{
    MouseHandler() = default;
    MouseHandler(MouseHandler const&) = delete;
    MouseHandler(MouseHandler&&) = delete;
    MouseHandler& operator=(MouseHandler const&) = delete;
    MouseHandler& operator=(MouseHandler &&) = delete;

    std::pair<sf::Vector2i,sf::Mouse::Button>  lastEvent;
    sf::Vector2i currentMousePosition = sf::Vector2i(-1,-1);
    public:
        static MouseHandler& instance() {
            static MouseHandler myInstance;
            return myInstance;
        }
        void handle(sf::Event& event){
            if(event.type == sf::Event::MouseMoved)
              currentMousePosition = sf::Vector2i(event.mouseMove.x,event.mouseMove.y);
            if(event.type == sf::Event::MouseButtonReleased)
                lastEvent = (std::make_pair(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),event.mouseButton.button));
        }

        sf::Vector2i getMousePosition(){
            return lastEvent.first;
        }
        sf::Vector2i getCurrentMousePosition(){
            return currentMousePosition;
        }
        sf::Mouse::Button getButton(){
            return lastEvent.second;
        }
        void clear(){
            lastEvent = std::make_pair(sf::Vector2i(-1,-1),sf::Mouse::Button::Middle);
        }

};

#endif // MOUSEHANDLER_H
