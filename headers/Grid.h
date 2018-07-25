#ifndef GRID_H
#define GRID_H

#include <SFML/System.hpp>

class Grid{
protected:
    sf::Vector2i position = sf::Vector2i(0,0);
    sf::Vector2f cellSize = sf::Vector2f(0,0);
    sf::Vector2i gridSize = sf::Vector2i(0,0);
public:
    virtual sf::Vector2i getPosition() const {return position;}
    virtual sf::Vector2f getCellSize() const {return cellSize;}
    virtual sf::Vector2i getGridSize() const {return gridSize;}
};


class GridPositioner{
    Grid & grid;
public:
    GridPositioner(Grid& g) : grid(g) {}
    sf::Vector2i getCellUnder(sf::Vector2i position){
        if(position.x < grid.getPosition().x || position.y < grid.getPosition().y)
            return sf::Vector2i(-1,-1);
        int posx = (position.x - grid.getPosition().x) / grid.getCellSize().x;
        int posy = (position.y - grid.getPosition().y) / grid.getCellSize().y;
        if(posx >= grid.getGridSize().x || posy >= grid.getGridSize().y)
            return sf::Vector2i(-1,-1);
        return sf::Vector2i(posx,posy);
    }
};
#endif // GRID_H
