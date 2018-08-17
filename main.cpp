#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cassert>
#include <set>
#include "Checker.h"
#include "ResourceManager.h"
#include "Checkboard.h"
#include "MoveController.h"
#include "MouseHandler.h"
#include "WindowResizer.h"
#include "Button.h"
#include "Player.h"
using namespace std;


class CheckersArranger{
    Checkboard& checkboard;
public:
    CheckersArranger(Checkboard& c) : checkboard(c){}

    void arrange(int heightBlack, int heightWite, bool blackUpper = true){
        assert(heightBlack + heightWite <= checkboard.getHeight());

        Checker upper_pawn = black_queen;
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



int main(){
    ResizableRenderWindow window(sf::VideoMode(1280, 800), "Checkers! ");
    window.setStdSize(window.getSize());
    window.setBackgroundColor(sf::Color(190,190,190));
    TextureManager::instance().loadAll();
    FontManager::instance().loadAll();

    int selectedCellStyle = 0;
    int selectedCheckersStyle = 0;

    ifstream dataFile;
    dataFile.open("data/save.dat");
    dataFile >> selectedCellStyle;
    dataFile >> selectedCheckersStyle;
    dataFile.close();

    while(window.isOpen()){
        sf::Color mainMenuButtonColor = sf::Color(0,155,155);
        Button b1(sf::IntRect(500,100,300,100),mainMenuButtonColor,"Nowa gra"),
               b2(sf::IntRect(500,300,300,100),mainMenuButtonColor,"Opcje"),
               b3(sf::IntRect(500,500,300,100),mainMenuButtonColor,L"Wyjście");

        window.addDrawable(&b1);
        window.addDrawable(&b2);
        window.addDrawable(&b3);

        bool runSettingsMenu = false;
        while (window.isOpen()){
            MouseHandler::instance().clear();
            sf::Event event;
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed)
                    window.close();
                MouseHandler::instance().handle(event);
            }
            if(b1.isClicked(window))
                break;

            if(b2.isClicked(window)){
                runSettingsMenu = true;
                break;
            }
            if(b3.isClicked(window))
                window.close();

            window.render();
        }
        window.clearDrawables();

        if(runSettingsMenu){
            Button backToMainMenu(sf::IntRect(1000,700,200,80),mainMenuButtonColor,L"Wróć");
            window.addDrawable(&backToMainMenu);
            vector<Button> cellsStyleSprites;
            vector<Button> checkersStyleSprites;
            for(int i=0;;i++){
                if(TextureManager::instance().contains("cells " + toStr(i))){
                    cellsStyleSprites.push_back(Button(sf::IntRect(50,50+120*i,180,90)));
                    cellsStyleSprites.back().setTexture(&TextureManager::instance().get("cells " + toStr(i)));
                    cellsStyleSprites.back().setOutline(1);
                }
                else
                    break;
            }

            for(auto &a :cellsStyleSprites)
                window.addDrawable(&a);
            for(int i=0;;i++){
                if(TextureManager::instance().contains("whitePawn " + toStr(i))){
                    checkersStyleSprites.push_back(Button(sf::IntRect(500,50+120*i,90,90)));
                    checkersStyleSprites.back().setTexture(&TextureManager::instance().get("whitePawn " + toStr(i)));
                    checkersStyleSprites.back().setOutline(1);
                }
                else
                    break;
            }
            for(auto &a :checkersStyleSprites)
                window.addDrawable(&a);



            cellsStyleSprites[selectedCellStyle].setOutline(5,sf::Color(40,140,240));
            checkersStyleSprites[selectedCheckersStyle].setOutline(5,sf::Color(40,140,240));

            while (window.isOpen()){
                MouseHandler::instance().clear();
                sf::Event event;
                while (window.pollEvent(event)){
                    if (event.type == sf::Event::Closed)
                        window.close();
                    MouseHandler::instance().handle(event);
                }
                bool changed = false;
                for(int i=0;i<cellsStyleSprites.size();i++){
                    if(cellsStyleSprites[i].isClicked(window)){
                        cellsStyleSprites[selectedCellStyle].setOutline(1);
                        selectedCellStyle = i;
                        cellsStyleSprites[selectedCellStyle].setOutline(5,sf::Color(40,140,240));
                        changed = true;
                    }
                }
                for(int i=0;i<checkersStyleSprites.size();i++){
                    if(checkersStyleSprites[i].isClicked(window)){
                        checkersStyleSprites[selectedCheckersStyle].setOutline(1);
                        selectedCheckersStyle = i;
                        checkersStyleSprites[selectedCheckersStyle].setOutline(5,sf::Color(40,140,240));
                        changed = true;
                    }
                }
                if(changed){
                    ofstream dataFile;
                    dataFile.open("data/save.dat");
                    dataFile << selectedCellStyle<<" " <<selectedCheckersStyle;
                    dataFile.close();
                }
                if(backToMainMenu.isClicked(window))
                    break;
                window.render();
            }
            window.clearDrawables();
        }
        else{
            Checkboard checkboard;
            checkboard.setSize(8,8);
            checkboard.drawer.setCellStyle(selectedCellStyle);
            checkboard.drawer.setCheckersStyle(selectedCheckersStyle);

            CheckersArranger arranger(checkboard);
            arranger.arrange(1,1);

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

            mainText.setColor(sf::Color(90,90,90));
            commentText.setColor(sf::Color(90,90,90));

            mainText.setCharacterSize(64);
            commentText.setCharacterSize(25);

            mainText.setString(winnerMessage);
            commentText.setString(endOfGameReason);

            Button exitButton(sf::IntRect(900,650,250,80),sf::Color(0,155,155),L"Wyjście");
            Button mainMenuButton(sf::IntRect(900,550,250,80),sf::Color(0,155,155),L"Menu Główne");

            window.addDrawable(&checkboard.drawer);
            window.addDrawable(&mainText);
            window.addDrawable(&commentText);
            window.addDrawable(&mainMenuButton);
            window.addDrawable(&exitButton);

            while (window.isOpen()){
                MouseHandler::instance().clear();
                sf::Event event;
                while (window.pollEvent(event)){
                    if (event.type == sf::Event::Closed)
                        window.close();
                    MouseHandler::instance().handle(event);
                }
                if(exitButton.isClicked(window))
                    window.close();
                if(mainMenuButton.isClicked(window))
                    break;
                window.render();
            }
            window.clearDrawables();
        }
    }

    return 0;
}
