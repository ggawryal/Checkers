#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <set>
#include "Checker.h"
#include "MaxiJumpSequenceFinder.h"
#include "TextureManager.h"
#include "Grid.h"
#include "Checkboard.h"
#include "MoveController.h"
#include "MouseHandler.h"
#include "Highlighter.h"
using namespace std;


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
    void customArrange(vector<vector<int> > board){
        assert(board[0].size() == checkboard.getWidth());
        assert(board.size() == checkboard.getHeight());

        vector<vector<Checker> > customArrangeChecker(board[0].size(),vector<Checker>(board.size()));
        for(int i=0;i<board[0].size();i++){
            for(int j=0;j<board.size();j++)
                customArrangeChecker[j][i] = Checker(board[i][j]);
        }
        for(int i=0;i<customArrangeChecker.size();i++)
            for(int j=0;j<customArrangeChecker[i].size();j++)
                if(customArrangeChecker[i][j] != Checker::empty)
                    checkboard.addChecker(i,j,customArrangeChecker[i][j]);
    }
};

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
    sf::RenderWindow& window;
    sf::Vector2i currentPawn = sf::Vector2i(-1,-1);
public:
    HumanPlayer(sf::RenderWindow& rw, GridPositioner& gd, MoveController& mv, Highlighter& hl,shared_ptr<Rules> rul) :gridPositioner(gd), window(rw), moveController(mv), highlighter(hl) {rules = rul;}
    virtual ~HumanPlayer(){}

    virtual void onBeginOfTurn() {}
    virtual void onEndOfTurn() {
        highlighter.resetCheckerHighlight(currentPawn.x,currentPawn.y);
        currentPawn = sf::Vector2i(-1,-1);
    }


    virtual void onTurn() override{
        if(gridPositioner.getCellUnder(MouseHandler::instance().getCurrentMousePosition()) != sf::Vector2i(-1,-1)){
            if(MouseHandler::instance().getButton() == sf::Mouse::Left){
                if(currentPawn == sf::Vector2i(-1,-1)){
                    auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                    if(isMyChecker(playsWhite(), moveController.getChecker(cell.x,cell.y)) && rules->existAnyCorrectMoveWith(cell.x,cell.y)){
                        currentPawn = cell;
                        highlighter.highlightChecker(currentPawn.x,currentPawn.y,sf::Color::Green);
                    }
                }
                else{
                    auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                    Quad q = Quad(currentPawn,cell);
                    if(rules->isCorrectMove(q)){
                       moveController.move(currentPawn.x,currentPawn.y,cell.x,cell.y);
                       currentPawn = cell;
                    }
                }
            }
            if(MouseHandler::instance().getButton() == sf::Mouse::Right && moveController.countOfJumpedOverCheckers() == 0){
                highlighter.resetCheckerHighlight(currentPawn.x,currentPawn.y);
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
    Highlighter highlighter(checkboard);

    shared_ptr<Rules> rules(new ClassicRules(mv));

    vector<unique_ptr<Player> > player;
    player.push_back(unique_ptr<Player>(new HumanPlayer(window,gp,mv,highlighter, rules)));
    player.push_back(unique_ptr<Player>(new HumanPlayer(window,gp,mv,highlighter, rules)));

    player[0]->setColor(true);
    player[1]->setColor(false);
    int currentPlayerId = 0;

    while (window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            MouseHandler::instance().handle(event);
        }

        if(mv.isBlackOnTurn() != currentPlayerId){
            player[currentPlayerId]->onEndOfTurn();
            currentPlayerId ^= 1;
            player[currentPlayerId]->onBeginOfTurn();
        }
        player[currentPlayerId]->onTurn();

        if(rules->isEndOfGame())
            break;

        if(currentPlayerId == 0)
            window.clear(sf::Color(220,170,50));
        else
            window.clear(sf::Color(100,100,100));
        window.draw(checkboard.drawer);
        window.display();
    }
    if(rules->blackWon())
        cout<<"Wygraly czarne\n";
    if(rules->whiteWon())
        cout<<"Wygraly biale\n";
    if(rules->isDraw())
        cout<<"Remis\n";
    return 0;
}
