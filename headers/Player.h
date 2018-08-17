#ifndef PLAYER_H
#define PLAYER_H
#include "Grid.h"
#include "MoveController.h"
#include "Highlighter.h"
#include "WindowResizer.h"


class Player{
    bool color = true;
    public:
        virtual void onBeginOfTurn() = 0;
        virtual void onTurn() = 0;
        virtual void onEndOfTurn() = 0;
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
    Highlighter& highlighter;
    shared_ptr<Rules> rules;
    ResizableRenderWindow& window;
    sf::Vector2i currentPawn = sf::Vector2i(-1,-1);
public:
    HumanPlayer(ResizableRenderWindow& rw, GridPositioner& gd, MoveController& mv, Highlighter& hl,shared_ptr<Rules> rul) :gridPositioner(gd), window(rw), moveController(mv), highlighter(hl) {rules = rul;}
    virtual ~HumanPlayer(){}

    virtual void onBeginOfTurn() {}
    virtual void onEndOfTurn() {
        if(currentPawn != sf::Vector2i(-1,-1))
            highlighter.resetCheckerHighlight(currentPawn.x,currentPawn.y);
        currentPawn = sf::Vector2i(-1,-1);
    }


    virtual void onTurn() override{
        if(gridPositioner.getCellUnder(window.mapPixelToStd(MouseHandler::instance().getCurrentMousePosition())) != sf::Vector2i(-1,-1)){
            if(MouseHandler::instance().getButton() == sf::Mouse::Left){
                if(currentPawn == sf::Vector2i(-1,-1)){
                    auto cell = gridPositioner.getCellUnder(window.mapPixelToStd(sf::Mouse::getPosition(window)));
                    if(isMyChecker(playsWhite(), moveController.getChecker(cell.x,cell.y)) && rules->existAnyCorrectMoveWith(cell.x,cell.y)){
                        currentPawn = cell;
                        highlighter.highlightChecker(currentPawn.x,currentPawn.y,sf::Color(0,255,128));
                    }
                }
                else{
                    auto cell = gridPositioner.getCellUnder(window.mapPixelToStd(sf::Mouse::getPosition(window)));
                    Quad q = Quad(currentPawn,cell);
                    if(rules->isCorrectMove(q)){
                       moveController.move(currentPawn.x,currentPawn.y,cell.x,cell.y);
                       currentPawn = cell;
                    }
                }
            }
            if(MouseHandler::instance().getButton() == sf::Mouse::Right && moveController.countOfJumpedOverCheckers() == 0){
                if(currentPawn != sf::Vector2i(-1,-1))
                    highlighter.resetCheckerHighlight(currentPawn.x,currentPawn.y);
                currentPawn = sf::Vector2i(-1,-1);
            }

            MouseHandler::instance().clear();
        }
    }
};

#endif // PLAYER_H
