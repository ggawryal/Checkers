#include "MoveController.h"
#include <iostream>
bool MoveController::isSimpleMovingUp(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false){
        return false;
    }
    if(abs(x2-x1) == 1 && y2-y1 == -1)
        return true;
    return false;
}

bool MoveController::isSimpleMovingDown(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false)
        return false;
    if(abs(x2-x1) == 1 && y2-y1 == 1)
        return true;
    return false;
}

bool MoveController::isSimpleMoving(int x1,int y1,int x2,int y2){
    if(MoveController::isSimpleMovingUp(x1,y1,x2,y2) || MoveController::isSimpleMovingDown(x1,y1,x2,y2))
        return true;
    return false;
}

bool MoveController::isSimpleJumpingUp(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false)
        return false;
    if(abs(x2-x1) == 2 && y2-y1 == -2 && isEnemyChecker(whiteOnTurn,checkboard.getChecker((x1+x2)/2,(y1+y2)/2)))
        return true;
    return false;
}

bool MoveController::isSimpleJumpingDown(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false)
        return false;
    if(abs(x2-x1) == 2 && y2-y1 == 2 && isEnemyChecker(whiteOnTurn,checkboard.getChecker((x1+x2)/2,(y1+y2)/2)))
        return true;
    return false;
}

bool MoveController::isSimpleJumping(int x1,int y1,int x2,int y2){
    if(MoveController::isSimpleJumpingUp(x1,y1,x2,y2) || MoveController::isSimpleJumpingDown(x1,y1,x2,y2))
        return true;
    return false;
}
