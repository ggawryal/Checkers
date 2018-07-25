#ifndef PLAYER_H
#define PLAYER_H
 #include "Grid.h"

class Player{
    bool color;
    public:
        Player();
        virtual ~Player();
        virtual void processTurn();
        bool playsWhite(){
            return color;
        }
        bool setColor(bool white){
            color = white;
        }

};

class HumanPlayer : public Player{
    GridPositioner gridPositioner;

    virtual void processTurn() override{
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

        }
    }
};

#endif // PLAYER_H
