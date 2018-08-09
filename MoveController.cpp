#include "MoveController.h"
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


bool MoveController::isLongMoving(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false)
        return false;

    if(abs(x2 - x1) != abs(y2 - y1)) //is not diagonal
        return false;

    sf::Vector2i step = sf::Vector2i((x2-x1) / abs(x2-x1), (y2-y1) / abs(y2-y1));
    do{
        x1 += step.x;
        y1 += step.y;
        if(checkboard.getChecker(x1,y1) != Checker::empty)
            return false;
    }while(x1 != x2 && y1 != y2);

    return true;
}

bool MoveController::isLongJumping(int x1,int y1,int x2,int y2){
    Checker cell1 = checkboard.getChecker(x1,y1);
    Checker cell2 = checkboard.getChecker(x2,y2);
    if(cell1 == Checker::empty || cell2 != Checker::empty)
        return false;
    if(isMyChecker(whiteOnTurn,cell1) == false)
        return false;

    if(abs(x2 - x1) != abs(y2 - y1)) //is not diagonal
        return false;

    sf::Vector2i step = sf::Vector2i((x2-x1) / abs(x2-x1), (y2-y1) / abs(y2-y1));
    int enemyCheckersCounter = 0;
    do{
        x1 += step.x;
        y1 += step.y;
        Checker checker = checkboard.getChecker(x1,y1);

        if(isMyChecker(whiteOnTurn,checker) || checker == Checker::jumped_over)
            return false;

        if(isEnemyChecker(whiteOnTurn,checker))
            enemyCheckersCounter++;

    }while(x1 != x2 && y1 != y2);
    if(enemyCheckersCounter == 1)
        return true;
    return false;
}


void MoveController::move(int x1,int y1,int x2,int y2){
    if(jumpingCheckerStartPos == sf::Vector2i(-1,-1))
        jumpingCheckerStartPos = sf::Vector2i(x1,y1);
    checkboard.moveChecker(x1,y1,x2,y2);
    sf::Vector2i step = sf::Vector2i((x2-x1) / abs(x2-x1), (y2-y1) / abs(y2-y1));

    while(x1 != x2 && y1 != y2){
        if(checkboard.getChecker(x1,y1) != Checker::empty)
            checkboard.markPawnAsJumpedOver(x1,y1);
        x1 += step.x;
        y1 += step.y;
    }
   if(checkboard.jumpedOverCheckers.size() == 0 ||
       mjsf.getMaxiSequenceAfter(x2,y2,jumpingCheckerStartPos.x,jumpingCheckerStartPos.y,isPawn(checkboard.getChecker(x2,y2)),checkboard.jumpedOverCheckers) == 0){
        if((whiteOnTurn && y2 == 0) || (whiteOnTurn == false && y2 == checkboard.getHeight()-1))
            promote(x2,y2);
        nextTurn();
    }

}
