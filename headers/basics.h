#ifndef BASICS_H
#define BASICS_H
#include <SFML/System.hpp>

struct Quad{
    int x1,y1,x2,y2;
    Quad(int X1=0,int Y1=0,int X2=0,int Y2=0) : x1(X1),y1(Y1),x2(X2),y2(Y2){}
    Quad(sf::Vector2i v1, sf::Vector2i v2) : Quad(v1.x,v1.y,v2.x,v2.y){}
};

#endif // BASICS_H
