#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <set>
#include "Checker.h"
#include "MaxiJumpSequenceFinder.h"
#include "ResourceManager.h"
#include "Grid.h"
#include "Checkboard.h"
#include "MoveController.h"
#include "MouseHandler.h"
#include "Highlighter.h"
#include "WindowResizer.h"
#include "Button.h"

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


int main(){
    ResizableRenderWindow window(sf::VideoMode(1280, 800), "Checkers!");
    window.setStdSize(window.getSize());

    TextureManager::instance().loadAll();
    FontManager::instance().loadAll();

    Checkboard checkboard;
    checkboard.setSize(8,8);

    CheckersArranger arranger(checkboard);
    arranger.arrange(1,3);

    checkboard.drawer.setPosition(sf::Vector2i(50,50));
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
            if (event.type == sf::Event::Closed){
                window.close();
                exit(0);
            }
            MouseHandler::instance().handle(event);
        }

        if(mv.isBlackOnTurn() != currentPlayerId){
            player[currentPlayerId]->onEndOfTurn();
            currentPlayerId ^= 1;
            if(rules->isEndOfGame())
                break;
            player[currentPlayerId]->onBeginOfTurn();
        }
        player[currentPlayerId]->onTurn();



        if(currentPlayerId == 0)
            window.clear(sf::Color(220,170,50));
        else
            window.clear(sf::Color(100,100,100));
        window.draw(checkboard.drawer);
        window.display();
    }


    sf::String winnerMessage = "";
    sf::String endOfGameReason = "";

    if(rules->whiteWon())
        winnerMessage = L"Wygrały białe";
    if(rules->blackWon())
        winnerMessage = L"Wygrały czarne";
    if(rules->isDraw())
        winnerMessage = L"Remis";

    if(rules->getEndOfGameReason() == 0)
        endOfGameReason = L"Zbito wszystkie\nfigury przeciwnika";

    if(rules->getEndOfGameReason() == 1)
        endOfGameReason = L"Zablokowano wszystkie\nfigury przeciwnika";

    if(rules->getEndOfGameReason() == 2)
        endOfGameReason = L"Wykonano po 15 posunięć\nbez bicia ani ruchu pionem";



    sf::Text mainText, commentText;
    mainText.setFont(FontManager::instance().get("arial"));
    commentText.setFont(FontManager::instance().get("arial"));

    mainText.setPosition(800,50);
    commentText.setPosition(800,150);

    mainText.setColor(sf::Color(190,190,190));
    commentText.setColor(sf::Color(190,190,190));

    mainText.setCharacterSize(64);
    commentText.setCharacterSize(25);

    mainText.setString(winnerMessage);
    commentText.setString(endOfGameReason);

    Button button(sf::IntRect(900,650,250,80));
    button.setBackgroundColor(sf::Color(0,155,155));
    button.setText(L"Wyjście");



    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            MouseHandler::instance().handle(event);
        }
        if(MouseHandler::instance().getButton() == sf::Mouse::Left &&
           button.isPointInside(window.mapPixelToStd(MouseHandler::instance().getCurrentMousePosition()))){

            window.close();
        }
        MouseHandler::instance().clear();
        window.clear(sf::Color(70,70,70));
        window.draw(checkboard.drawer);
        window.draw(mainText);
        window.draw(commentText);
        window.draw(button);
        window.display();
    }


    return 0;
}
