#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include "MaxiJumpSequenceFinder.h"
#include "TextureManager.h"
#include "Grid.h"
using namespace std;

struct NotImplementedException : public std::logic_error{
    NotImplementedException() : std::logic_error("Function not yet implemented") { };
};

enum Checkers {empty = 0, white_pawn = 1, white_queen = 2, black_pawn = 3, black_queen = 4};

struct CheckerOnGrid{
    int x,y;
    sf::Sprite sprite;
};

class CheckBoardDrawer : public sf::Drawable, public Grid{
public:
    vector<vector<sf::Sprite> > checkboard;
    vector<CheckerOnGrid > checkers;

    void setPosition(sf::Vector2i pos){
        position = pos;
    }
    virtual sf::Vector2i getGridSize() const override{
        if(checkboard.size() == 0)
            return sf::Vector2i(0,0);
        else
            return sf::Vector2i(checkboard.size(),checkboard[0].size());
    }
    void resizeBoard(int x,int y){
        checkboard.clear();
        checkboard.resize(x);
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                checkboard[i].push_back(sf::Sprite());
                if((i+j) % 2  == 1)
                    checkboard[i].back().setTexture(TextureManager::instance().get("blackCell 1"));
                else
                    checkboard[i].back().setTexture(TextureManager::instance().get("whiteCell 1"));
            }
        }
        if(x > 0 && y > 0 && cellSize.x == 0)
            cellSize = sf::Vector2f(checkboard[0][0].getTexture()->getSize());


    }
    void fixSprites(){
        if(checkboard.size() == 0 || checkboard[0].size() == 0)
            return;
        for(int i=0;i<checkboard.size();i++){
            for(int j=0;j<checkboard[i].size();j++){
                checkboard[i][j].setPosition(sf::Vector2f(position.x+i*cellSize.x,position.y+j*cellSize.y));
            }
        }
        for(auto &a:checkers)
            a.sprite.setPosition(sf::Vector2f(position.x+a.x*cellSize.x,position.y+a.y*cellSize.y));

    }
    void setCellSize(int x,int y){

        sf::Vector2u standardSize = checkboard[0][0].getTexture()->getSize();
        cellSize = sf::Vector2f(x,y);

        for(auto& column:checkboard)
            for(auto &sprite : column)
                sprite.setScale(cellSize.x/(float)standardSize.x, cellSize.y/(float)standardSize.y);
        for(auto &checker :checkers)
            checker.sprite.setScale(cellSize.x/(float)standardSize.x, cellSize.y/(float)standardSize.y);


        fixSprites();
    }
    void setImageSize(int x,int y){
        assert(checkboard.size() > 0);
        assert(checkboard[0].size() > 0);

        cellSize = sf::Vector2f(x/(float)(checkboard.size()), y/(float)(checkboard[0].size()));
        setCellSize(cellSize.x,cellSize.y);
    }

    void addChecker(int x,int y,Checkers type){
        checkers.push_back(CheckerOnGrid());
        CheckerOnGrid& newChecker = checkers.back();
        newChecker.x = x;
        newChecker.y = y;
        assert(type != Checkers::empty);
         if(type == Checkers::black_pawn)
            newChecker.sprite.setTexture(TextureManager::instance().get("blackPawn 1"));
        if(type == Checkers::black_queen)
            newChecker.sprite.setTexture(TextureManager::instance().get("blackQueen 1"));
        if(type == Checkers::white_pawn)
            newChecker.sprite.setTexture(TextureManager::instance().get("whitePawn 1"));
        if(type == Checkers::white_queen)
            newChecker.sprite.setTexture(TextureManager::instance().get("whiteQueen 1"));
    }
    void moveChecker(int x,int y,int x2, int y2){
        int pos = -1;
        for(int i=0;i<checkers.size();i++){
            if(checkers[i].x == x && checkers[i].y == y){
                pos = i;
                break;
            }
        }
        assert(pos != -1);
        checkers[pos].x = x2;
        checkers[pos].y = y2;
        checkers[pos].sprite.setPosition(sf::Vector2f(position.x + checkers[pos].x*cellSize.x, position.y + checkers[pos].y*cellSize.y));
    }
    void removeChecker(int x,int y){
        for(auto it=checkers.begin();it != checkers.end();it++){
            if(it->x == x && it->y == y){
                cout<<"erasing"<<endl;
                checkers.erase(it);
                break;
            }
        }
    }

    void updateSize(vector<vector<Checkers> >& board){
        if(board.size() != checkboard.size() || (board.size() > 0 && board[0].size() != checkboard[0].size()))
            resizeBoard(board.size(),board[0].size());

    }
    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override{
        for(int i=0;i<checkboard.size();i++)
            for(int j=0;j<checkboard[i].size();j++)
                target.draw(checkboard[i][j],states);

        for(auto &a:checkers)
            target.draw(a.sprite,states);

    }
};

class Checkboard{
    vector<vector<Checkers> > board;
public:
    CheckBoardDrawer drawer;
    int setSize(int x,int y){
        board.clear();
        board.resize(x,vector<Checkers>(y));
        drawer.fixSprites();
        drawer.updateSize(board);
    }
    int getWidth(){
        return board.size();
    }
    int getHeight(){
        return board.size() == 0 ? 0 : board[0].size();
    }
    Checkers getChecker(int x,int y){
        assert(x < getWidth());
        assert(y < getHeight());
        assert(x >=0);
        assert(y >= 0);
        return board[x][y];
    }
    void addChecker(int x,int y,Checkers type){
        assert(x < board.size());
        assert(board.size() > 0);
        assert(y < board[0].size());

        if(board[x][y] != Checkers::empty){
            cout<<"Warning: cell "<<x<<" "<<y<<" not empty, already "<<(int)board[x][y]<<" on it"<<endl;
            for(int i=0;i<drawer.checkers.size();i++){
                if(drawer.checkers[i].x == x && drawer.checkers[i].y == y){
                    drawer.checkers.erase(drawer.checkers.begin()+i);
                    break;
                }
            }
        }
        board[x][y] = type;
        drawer.addChecker(x,y,type);
    }
    void removeChecker(int x, int y){
        assert(x < board.size());
        assert(board.size() > 0);
        assert(y < board[0].size());

        if(board[x][y] == Checkers::empty)
            cout<<"Warning: trying to remove checker from cell "<<x<<" "<<y<<" which is already empty"<<endl;
        else{
            board[x][y] = Checkers::empty;
            drawer.removeChecker(x,y);
        }
    }
    void moveChecker(int x,int y,int x2,int y2){
        assert(board.size() > 0);
        assert(x < board.size());
        assert(y < board[0].size());
        assert(x2 < board.size());
        assert(y2 < board[0].size());

        assert(board[x][y] != Checkers::empty);
        assert(board[x2][y2] == Checkers::empty);

        board[x2][y2] = board[x][y];
        board[x][y] = Checkers::empty;
        drawer.moveChecker(x,y,x2,y2);;
    }
};

class CheckersArranger{
    Checkboard& checkboard;
public:
    CheckersArranger(Checkboard& c) : checkboard(c){}

    void arrange(int heightBlack, int heightWite, bool blackUpper = true){
        assert(heightBlack + heightWite <= checkboard.getHeight());

        Checkers upper_pawn = black_pawn;
        Checkers lower_pawn = white_pawn;

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

class MoveController{
    bool whiteOnTurn = true;
    Checkboard& checkboard;
public:
    MoveController(Checkboard&c) : checkboard(c){}
    Checkers getChecker(int x,int y){
        return checkboard.getChecker(x,y);
    }
    bool isLegalMove(int x1, int y1,int x2,int y2){
        Checkers cell1 = checkboard.getChecker(x1,y1);
        Checkers cell2 = checkboard.getChecker(x2,y2);
        cout<<"FSADGGAGAGDA "<<cell1<<" "<<cell2<<endl;
        if(cell1 == Checkers::empty || cell2 != Checkers::empty)
            return false;

        vector<Checkers> enemies,myCheckers;
        if(whiteOnTurn){
            enemies = {Checkers::black_pawn, Checkers::black_queen};
            myCheckers = {Checkers::white_pawn, Checkers::white_queen};
        }
        else{
            enemies = {Checkers::white_pawn, Checkers::white_queen};
            myCheckers = {Checkers::black_pawn, Checkers::black_queen};
        }
        cout<<"ch: "<<myCheckers[0]<<endl;
        if(cell1 == myCheckers[0]){ //pawn
            std::vector<std::pair<int,int> > directions = {{1,1},{-1,1},{-1,-1},{1,-1}};
            for(int i=0;i<directions.size();i++){
                if(x1 + 2*directions[i].first == x2 && y1 + 2*directions[i].second == y2){
                    if(checkboard.getChecker((x1+x2)/2,(y1+y2)/2) == enemies[0] ||
                        checkboard.getChecker((x1+x2)/2,(y1+y2)/2) == enemies[1])
                        return true;
                    return false;
                }
                if(x1 + directions[i].first == x2 && y1 + directions[i].second == y2)
                    return true;
            }
            return false;
        }
        if(cell1 == myCheckers[1]) //queen
            throw NotImplementedException();
        return false;
    }
    void move(int x1,int y1,int x2,int y2){
        checkboard.moveChecker(x1,y1,x2,y2);
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
        cout<<currentPawn.x<<" {{}] "<<currentPawn.y<<endl;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(currentPawn == sf::Vector2i(-1,-1)){
                auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                Checkers ch = moveController.getChecker(cell.x,cell.y);
                if(playsWhite()){
                    if(ch == Checkers::white_pawn || ch == Checkers::white_queen){
                        currentPawn = cell;
                        sf::sleep(sf::milliseconds(1000));
                    }
                }
            }
            else{
                auto cell = gridPositioner.getCellUnder(sf::Mouse::getPosition(window));
                cout<<cell.x<<" "<<cell.y<<endl;
                if(moveController.isLegalMove(currentPawn.x,currentPawn.y,cell.x,cell.y)){
                   moveController.move(currentPawn.x,currentPawn.y,cell.x,cell.y);
                   sf::sleep(sf::milliseconds(1000));
                   currentPawn = sf::Vector2i(-1,-1);
                }
            }
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
