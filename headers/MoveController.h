#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H
#include "Checker.h"
#include "Checkboard.h"
#include "basics.h"

class MoveController{
    bool whiteOnTurn = true;
    Checkboard& checkboard;
public:
    bool isWwhiteOnTurn(){
        return whiteOnTurn;
    }
    bool isBlackOnTurn(){
        return !whiteOnTurn;
    }

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

    void move(int x1,int y1,int x2,int y2){
        checkboard.moveChecker(x1,y1,x2,y2);
        whiteOnTurn ^= 1;
    }
};

#endif // MOVECONTROLLER_H
