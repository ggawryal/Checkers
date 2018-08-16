#ifndef RESIZABLERENDERWINDOW_H
#define RESIZABLERENDERWINDOW_H
#include <SFML/Graphics.hpp>

class ResizableRenderWindow : public sf::RenderWindow{
    sf::Vector2i standardWindowSize;
    sf::Color bkgColor = sf::Color(0,0,0,255);
    std::vector<sf::Drawable*> drawables;
    public:

        ResizableRenderWindow()	: sf::RenderWindow(){}

        ResizableRenderWindow(sf::VideoMode mode, const sf::String & title, sf::Uint32 style = sf::Style::Default,
                              const sf::ContextSettings & 	settings = sf::ContextSettings()) : RenderWindow(mode,title,style,settings){}

        ResizableRenderWindow(sf::WindowHandle handle, const sf::ContextSettings & settings = sf::ContextSettings()) : RenderWindow(handle,settings) {}

        virtual ~ResizableRenderWindow()	{}

        void setStdSize(sf::Vector2u size){
            standardWindowSize = sf::Vector2i(size);
        }

        void close(){
            RenderWindow::close();
            exit(0);
        }
        void setBackgroundColor(sf::Color c){
            bkgColor = c;
        }
        void addDrawable(sf::Drawable* d){
            drawables.push_back(d);
        }
        void clearDrawables(){
            drawables.clear();
        }

        void render(){
            clear(bkgColor);
            for(auto& a : drawables)
                draw(*a);
            display();
        }

        template<class T>
        sf::Vector2<T> mapPixelToStd(T x,T y){
            sf::Vector2i currentSize = (sf::Vector2i)getSize();
            return sf::Vector2<T>(standardWindowSize.x*x/currentSize.x, standardWindowSize.y*y/currentSize.y);
        }
        template<class T>
        sf::Vector2<T> mapPixelToStd(sf::Vector2<T> pixel){
            sf::Vector2i currentSize = (sf::Vector2i)getSize();
            return sf::Vector2<T>(standardWindowSize.x*pixel.x/currentSize.x, standardWindowSize.y*pixel.y/currentSize.y);
        }
};

#endif // RESIZABLERENDERWINDOW_H
