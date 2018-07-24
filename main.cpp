#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include "MaxiJumpSequenceFinder.h"
using namespace std;

class TextureManager{
    TextureManager() = default;
    TextureManager(TextureManager const&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager const&) = delete;
    TextureManager& operator=(TextureManager &&) = delete;

    vector<sf::Texture> textures;
    map<string,int> mapping;

public:
   static TextureManager& instance() {
        static TextureManager myInstance;
        return myInstance;
    }

    void load(const string name, const string filename, const sf::IntRect area = sf::IntRect()){
        textures.push_back(sf::Texture());
        textures.back().loadFromFile(filename,area);
        mapping[name] = textures.size()-1;
    }
    void loadAll(){
        load("blackCell 1","data/graph/board.png",sf::IntRect(0,0,89,89));
        load("whiteCell 1","data/graph/board.png",sf::IntRect(89,0,89,89));

        load("blackPawn 1","data/graph/checkers.png",sf::IntRect(0,0,89,89));
        load("whitePawn 1","data/graph/checkers.png",sf::IntRect(89,0,89,89));

        load("blackQueen 1","data/graph/checkers.png",sf::IntRect(0,89,89,89));
        load("whiteQueen 1","data/graph/checkers.png",sf::IntRect(89,89,89,89));
    }
    sf::Texture& get(const string& name){
        return textures[mapping[name]];
    }

};

enum Checkers {empty = 0, white_pawn = 1, white_queen = 2, black_pawn = 3, black_queen = 4};

class CheckBoardDrawer : public sf::Drawable{
public:
    vector<vector<sf::Sprite> > checkboard;
    sf::Vector2i position = sf::Vector2i(0,0);
    sf::Vector2f cellSize = sf::Vector2f(0,0);
   // vector<sf::Sprite > checkers;
    void resizeBoard(int x,int y){
        checkboard.clear();
        checkboard.resize(x);
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                checkboard[i].push_back(sf::Sprite());
                if((i+j) % 2  == 0)
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
    }
    void setCellSize(int x,int y){
        for(auto& column:checkboard)
            for(auto &sprite : column)
                sprite.setScale(x/(float)cellSize.x, y/(float)cellSize.y);
        cellSize = sf::Vector2f(x,y);
        fixSprites();
    }
    void setImageSize(int x,int y){
        assert(checkboard.size() > 0);
        assert(checkboard[0].size() > 0);
        sf::Vector2u standardSize = checkboard[0][0].getTexture()->getSize();

        cellSize = sf::Vector2f(x/(float)(checkboard.size()), y/(float)(checkboard[0].size()));

        for(auto& column:checkboard)
            for(auto &sprite : column)
                sprite.setScale(cellSize.x/(float)standardSize.x, cellSize.y/(float)standardSize.y);
        fixSprites();
    }

    void updateSize(vector<vector<Checkers> >& board){
        if(board.size() != checkboard.size() || (board.size() > 0 && board[0].size() != checkboard[0].size()))
            resizeBoard(board.size(),board[0].size());

    }

    virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override{
        for(int i=0;i<checkboard.size();i++){
            for(int j=0;j<checkboard[i].size();j++){
                target.draw(checkboard[i][j],states);
                //target.draw(checkers[i][j],states);
            }
        }
    }
};

class Checkboard{
    vector<vector<Checkers> > board;
public:
    CheckBoardDrawer drawer;
    int setSize(int x,int y){
        board.clear();
        board.resize(x,vector<Checkers>(y));
        update();
    }
    int getWidth(){
        return board.size();
    }
    int getHeight(){
        return board.size() == 0 ? 0 : board[0].size();
    }
    void update(){
        drawer.fixSprites();
        drawer.updateSize(board);
    }
};







int main(){
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Checkers!");

    TextureManager& textureManager = TextureManager::instance();
    textureManager.loadAll();
    Checkboard checkboard;
    checkboard.setSize(8,8);
    checkboard.drawer.setCellSize(80,80);
    //checkboard.drawer.setImageSize(700,700);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear(sf::Color::Yellow);
        window.draw(checkboard.drawer);
        window.display();
    }

    return 0;
}
