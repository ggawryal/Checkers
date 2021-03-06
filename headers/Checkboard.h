#ifndef CHECKBOARD_H
#define CHECKBOARD_H
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "basics.h"
#include "Grid.h"
#include <vector>
#include <cassert>
#include <iostream>

struct CheckerSpriteOnGrid{
    int x,y;
    Checker type;
    sf::Sprite sprite;
    friend bool operator<(const CheckerSpriteOnGrid&a,const CheckerSpriteOnGrid&b){
        if(a.x == b.x)
            return a.y < b.y;
        return a.x < b.x;
    }
};

class CheckBoardDrawer : public sf::Drawable, public Grid{
    int cellStyle = 0, checkersStyle = 0;
    void updateTextureOfChecker(CheckerSpriteOnGrid& checker){
        const Checker& type = checker.type;
        if(type == Checker::black_pawn)
            checker.sprite.setTexture(TextureManager::instance().get("blackPawn " + toStr(checkersStyle)));
        if(type == Checker::black_queen)
            checker.sprite.setTexture(TextureManager::instance().get("blackQueen " + toStr(checkersStyle)));
        if(type == Checker::white_pawn)
            checker.sprite.setTexture(TextureManager::instance().get("whitePawn " + toStr(checkersStyle)));
        if(type == Checker::white_queen)
            checker.sprite.setTexture(TextureManager::instance().get("whiteQueen " + toStr(checkersStyle)));
    }

public:
    std::vector<std::vector<sf::Sprite> > checkboard;
    std::vector<CheckerSpriteOnGrid > checkers;

    void setCellStyle(int styleIndex){
        if(cellStyle == styleIndex)
            return;
        cellStyle = styleIndex;

        for(int i=0;i<getGridSize().x;i++){
            for(int j=0;j<getGridSize().y;j++){
                if((i+j) % 2  == 1)
                    checkboard[i][j].setTexture(TextureManager::instance().get("blackCell " + toStr(cellStyle)));
                else
                    checkboard[i][j].setTexture(TextureManager::instance().get("whiteCell " + toStr(cellStyle)));
            }
        }
    }
    void setCheckersStyle(int styleIndex){
        if(checkersStyle == styleIndex)
            return;
        checkersStyle = styleIndex;
        for(auto &a : checkers)
            updateTextureOfChecker(a);
    }

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
                    checkboard[i].back().setTexture(TextureManager::instance().get("blackCell " + toStr(cellStyle)));
                else
                    checkboard[i].back().setTexture(TextureManager::instance().get("whiteCell " + toStr(cellStyle)));
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

    void addChecker(int x,int y,Checker type){
        checkers.push_back(CheckerSpriteOnGrid());
        CheckerSpriteOnGrid& newChecker = checkers.back();
        newChecker.type = type;
        newChecker.x = x;
        newChecker.y = y;
        assert(type != Checker::empty);
        updateTextureOfChecker(newChecker);

        sf::Vector2u standardSize = checkboard[0][0].getTexture()->getSize();
        newChecker.sprite.setScale(cellSize.x/(float)standardSize.x, cellSize.y/(float)standardSize.y);

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

    void markChecker(int x,int y,sf::Color color){
        int pos = -1;
        for(int i=0;i<checkers.size();i++){
            if(checkers[i].x == x && checkers[i].y == y){
                pos = i;
                break;
            }
        }
        assert(pos != -1);
        checkers[pos].sprite.setColor(color);
    }

    void removeChecker(int x,int y){
        for(auto it=checkers.begin();it != checkers.end();it++){
            if(it->x == x && it->y == y){
                checkers.erase(it);
                break;
            }
        }
    }

    void updateSize(std::vector<std::vector<Checker> >& board){
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
    std::vector<std::vector<Checker> > board;
public:
    std::vector<sf::Vector2i> jumpedOverCheckers;
    CheckBoardDrawer drawer;
    void setSize(int x,int y){
        board.clear();
        board.resize(x,std::vector<Checker>(y));
        drawer.fixSprites();
        drawer.updateSize(board);
    }
    int getWidth(){
        return board.size();
    }
    int getHeight(){
        return board.size() == 0 ? 0 : board[0].size();
    }
    bool isNotInBoard(int x,int y){
        return x < 0 || y < 0 || x >= getWidth() || y >= getHeight();
    }
    bool isInBoard(int x,int y){
        if(isNotInBoard(x,y))
            return false;
        return true;
    }
    Checker getChecker(int x,int y){
        assert(x < getWidth());
        assert(y < getHeight());
        assert(x >=0);
        assert(y >= 0);
        return board[x][y];
    }

    std::vector<std::vector<Checker> > getBoard() const{
        return board;
    }

    void addChecker(int x,int y,Checker type){
        assert(x < board.size());
        assert(board.size() > 0);
        assert(y < board[0].size());

        if(board[x][y] != Checker::empty){
            std::cerr<<"Warning: cell "<<x<<" "<<y<<" not empty, already "<<(int)board[x][y]<<" on it"<<std::endl;
            for(int i=0;i<drawer.checkers.size();i++){
                if(drawer.checkers[i].x == x && drawer.checkers[i].y == y){
                    drawer.checkers.erase(drawer.checkers.begin()+i);
                    break;
                }
            }
        }
        board[x][y] = type;
        drawer.addChecker(x,y,type);
        drawer.fixSprites();
    }
    void deleteChecker(int x, int y){
        assert(x < board.size());
        assert(board.size() > 0);
        assert(y < board[0].size());

        if(board[x][y] == Checker::empty)
            std::cerr<<"Warning: trying to remove checker from cell "<<x<<" "<<y<<" which is already empty"<<std::endl;
        else{
            board[x][y] = Checker::empty;
            drawer.removeChecker(x,y);
        }
    }
    void markPawnAsJumpedOver(int x,int y){
        assert(x < board.size());
        assert(board.size() > 0);
        assert(y < board[0].size());

        if(board[x][y] == Checker::empty)
            std::cerr<<"Warning: trying to mark checker from cell "<<x<<" "<<y<<" which is already empty"<<std::endl;
        else{
            board[x][y] = Checker::jumped_over;
            jumpedOverCheckers.push_back(sf::Vector2i(x,y));
            drawer.markChecker(x,y,sf::Color(100,100,100,150));
        }
    }

    void deleteAllMarkedAsJumpedOverCheckers(){
        for(auto checkerPos: jumpedOverCheckers)
            deleteChecker(checkerPos.x,checkerPos.y);
        jumpedOverCheckers.clear();
    }

    void moveChecker(int x,int y,int x2,int y2){
        assert(board.size() > 0);
        assert(x < board.size());
        assert(y < board[0].size());
        assert(x2 < board.size());
        assert(y2 < board[0].size());

        assert(board[x][y] != Checker::empty);
        assert(board[x2][y2] == Checker::empty);

        board[x2][y2] = board[x][y];
        board[x][y] = Checker::empty;
        drawer.moveChecker(x,y,x2,y2);;
    }

};

#endif // CHECKBOARD_H
