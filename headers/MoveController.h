#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H
#include "Checker.h"
#include "Checkboard.h"
#include "basics.h"

class MoveController{
    bool whiteOnTurn = true;
    Checkboard& checkboard;
public:
    MoveController(Checkboard&c) : checkboard(c){}
    Checker getChecker(int x,int y){
        return checkboard.getChecker(x,y);
    }

    bool isSimpleMovingUp(int x1,int y1,int x2,int y2);
    bool isSimpleMovingUp(Quad quad){
        return isSimpleMovingUp(quad.x1,quad.y1,quad.x2,quad.y2);
    }

    bool isSimpleMovingDown(int x1,int y1,int x2,int y2);
    bool isSimpleMovingDown(Quad quad){
        return isSimpleMovingDown(quad.x1,quad.y1,quad.x2,quad.y2);
    }

    bool isSimpleMoving(int x1,int y1,int x2,int y2);
    bool isSimpleMoving(Quad quad){
        return isSimpleMoving(quad.x1,quad.y1,quad.x2,quad.y2);
    }

    bool isSimpleJumpingUp(int x1,int y1,int x2,int y2);
    bool isSimpleJumpingUp(Quad quad){
        return isSimpleJumpingUp(quad.x1,quad.y1,quad.x2,quad.y2);
    }

    bool isSimpleJumpingDown(int x1,int y1,int x2,int y2);
    bool isSimpleJumpingDown(Quad quad){
        return isSimpleJumpingDown(quad.x1,quad.y1,quad.x2,quad.y2);
    }

    bool isSimpleJumping(int x1,int y1,int x2,int y2);
    bool isSimpleJumping(Quad quad){
        return isSimpleJumping(quad.x1,quad.y1,quad.x2,quad.y2);
    }


   /* bool isLegalMove(int x1, int y1,int x2,int y2){
        Checker cell1 = checkboard.getChecker(x1,y1);
        Checker cell2 = checkboard.getChecker(x2,y2);
        if(cell1 == Checker::empty || cell2 != Checker::empty)
            return false;

        vector<Checker> enemies,myCheckers;
        if(whiteOnTurn){
            enemies = {Checker::black_pawn, Checker::black_queen};
            myCheckers = {Checker::white_pawn, Checker::white_queen};
        }
        else{
            enemies = {Checker::white_pawn, Checker::white_queen};
            myCheckers = {Checker::black_pawn, Checker::black_queen};
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
    }*/
    void move(int x1,int y1,int x2,int y2){
        checkboard.moveChecker(x1,y1,x2,y2);
    }
};

#endif // MOVECONTROLLER_H
