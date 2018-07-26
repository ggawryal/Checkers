#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include "Checker.h"
#include "MaxiJumpSequenceFinder.h"
#include "TextureManager.h"
#include "Grid.h"
#include "Checkboard.h"
#include "MoveController.h"
#include "MouseHandler.h"
using namespace std;

struct NotImplementedException : public std::logic_error{
    NotImplementedException() : std::logic_error("Function not yet implemented") { };
};



class CheckersArranger{
    Checkboard& checkboard;
public:
    CheckersArranger(Checkboard& c) : checkboard(c){}

    void arrange(int heightBlack, int heightWite, bool blackUpper = true){
        assert(heightBlack + heightWite <= checkboard.getHeight());

        Checker upper_pawn = black_pawn;
        Checker lower_pawn = white_pawn;

        if(blackUpper == false){
            swap(upper_pawn,lower_pawn);
            swap(heightBlack,heightWite);
        }

        for(int i=0;i<heightBlack;i++)
            for(int j=0;j<checkboard.getWidth();j++)
                if((i+j) % 2 == 1)
                    checkboard.addChecker(j,i,upper_pawn);

        for(int i=checkboard.getHeight()-1; i>=checkboard.getHeight()-heightWite; i--)
            for(int j=0;j<checkboard.getWidth();j++)
                if((i+j) % 2 == 1)
                    checkboard.addChecker(j,i,lower_pawn);
    }
};

class Player{
    bool color = true;
    public:
        virtual void processTurn() = 0;
        bool playsWhite(){
            return color;
        }
        void setColor(bool white){
            color = white;
        }


};

class HumanPlayer : public Player{
    GridPositioner& gridPositioner;
    MoveController& moveController;
    sf::RenderWindow& window;
    sf::Vector2i currentPawn = sf::Vector2i(-1,-1);
public:
    HumanPlayer(sf::RenderWindow& rw, GridPositioner& gd, MoveController& mv) :gridPositioner(gd), window(rw), moveController(mv) {}
    virtual ~HumanPlayer(){}

    virtual void processTurn() override{
        if(gridPositioner.getCellUnder(MouseHandler::instance().getCurrentMousePosition()) != sf::Vector2i(-1,-1)){
            if(MouseHandler::instance().getButton() == sf::Mouse::Left){
                if(currentPawn == sf::Vector2i(-1,-1)){
                    auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                    Checker ch = moveController.getChecker(cell.x,cell.y);
                    if(playsWhite()){
                        if(ch == Checker::white_pawn || ch == Checker::white_queen){
                            currentPawn = cell;
                        }
                    }
                }
                else{
                    auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                    Quad q = Quad(currentPawn,cell);
                    if(moveController.isSimpleMovingUp(q)){
                       moveController.move(currentPawn.x,currentPawn.y,cell.x,cell.y);
                       currentPawn = sf::Vector2i(-1,-1);
                    }
                }
            }
            if(MouseHandler::instance().getButton() == sf::Mouse::Right){
                currentPawn = sf::Vector2i(-1,-1);
            }

            MouseHandler::instance().clear();
        }
    }
};


int main(){
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Checkers!");

    TextureManager& textureManager = TextureManager::instance();
    textureManager.loadAll();
    Checkboard checkboard;
    checkboard.setSize(8,8);

    CheckersArranger arranger(checkboard);
    arranger.arrange(3,3);

    checkboard.drawer.setPosition(sf::Vector2i(100,100));
    checkboard.drawer.setImageSize(700,700);

    GridPositioner gp(checkboard.drawer);

    MoveController mv(checkboard);
    HumanPlayer player(window,gp,mv);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            MouseHandler::instance().handle(event);
        }

        player.processTurn();
      //  if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
       //     checkboard.drawer.setImageSize(500,500);

        window.clear(sf::Color::Yellow);
        window.draw(checkboard.drawer);
        window.display();
    }

    return 0;
}
