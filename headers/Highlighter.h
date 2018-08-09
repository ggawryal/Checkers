#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H


class Highlighter{
    CheckBoardDrawer& drawer;
public:
    Highlighter(Checkboard& c) : drawer(c.drawer){}
    void highlightChecker(int x,int y,sf::Color color){
        drawer.markChecker(x,y,color);
    }
    void resetCheckerHighlight(int x,int y){
        drawer.markChecker(x,y,sf::Color(255,255,255,255));
    }

    void highlightCell(int x,int y,sf::Color color){
        drawer.checkboard[x][y].setColor(color);
    }
    void resetCellHighlight(int x,int y){
        drawer.checkboard[x][y].setColor(sf::Color(255,255,255,255));
    }

    void resetAllCheckersHighlights(){
        for(int i=0;i<drawer.checkers.size();i++)
            drawer.checkers[i].sprite.setColor(sf::Color(255,255,255,255));

    }

    void resetAllCellHighlights(){
        for(int i=0;i<drawer.getGridSize().x;i++)
            for(int j=0;j<drawer.getGridSize().y;j++)
                drawer.checkboard[i][j].setColor(sf::Color(255,255,255,255));
    }
};


#endif // HIGHLIGHTER_H
