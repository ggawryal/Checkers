#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include <SFML/System.hpp>
#include "Checker.h"
#include "Checkboard.h"
#include "basics.h"
#include "MaxiJumpSequenceFinder.h"


class MoveController{
    bool whiteOnTurn = true;
    Checkboard& checkboard;
    MaxiJumpSequenceFinder mjsf;
    sf::Vector2i jumpingCheckerStartPos;

    std::vector<Checker> moveCheckersType;
    std::vector<int> checkerCountAfterMoves;

    bool promote(int x1,int y1){
        assert(isMyChecker(whiteOnTurn,checkboard.getChecker(x1,y1)));
        checkboard.deleteChecker(x1,y1);
        if(whiteOnTurn)
            checkboard.addChecker(x1,y1,Checker::white_queen);
        else
            checkboard.addChecker(x1,y1,Checker::black_queen);
    }
    void nextTurn(){
        jumpingCheckerStartPos = sf::Vector2i(-1,-1);
        checkboard.deleteAllMarkedAsJumpedOverCheckers();
        whiteOnTurn ^= 1;
        mjsf.calculateForWholeBoard(checkboard.getBoard(),whiteOnTurn);
        checkerCountAfterMoves.push_back(getPawnCount(true) + getPawnCount(false));
    }

public:
    MoveController(Checkboard&c) : checkboard(c){
        mjsf.calculateForWholeBoard(checkboard.getBoard(),whiteOnTurn);
        jumpingCheckerStartPos = sf::Vector2i(-1,-1);
        checkerCountAfterMoves.push_back(getPawnCount(true) + getPawnCount(false));
    }

    int getMaxiJumpingSequenceLenght(int x,int y){
        assert(isMyChecker(whiteOnTurn,checkboard.getChecker(x,y)));
        if(jumpingCheckerStartPos == sf::Vector2i(-1,-1))
            return mjsf.getMaxiSequenceAfter(x,y,x,y, isPawn(checkboard.getChecker(x,y)),checkboard.jumpedOverCheckers);
        return mjsf.getMaxiSequenceAfter(x,y,jumpingCheckerStartPos.x,jumpingCheckerStartPos.y, isPawn(checkboard.getChecker(x,y)),checkboard.jumpedOverCheckers);
    }
    int getMaxiJumpingSequenceLenghtAfterJump(int x1,int y1,int x2,int y2){
        assert(isMyChecker(whiteOnTurn,checkboard.getChecker(x1,y1)));

        sf::Vector2i step = sf::Vector2i((x2-x1) / std::abs(x2-x1), (y2-y1) / std::abs(y2-y1));

        std::vector<sf::Vector2i> jumpedOverCheckersAfterNextJump = checkboard.jumpedOverCheckers;
        int cx1 = x1, cy1 = y1;
        cx1 += step.x; cy1 += step.y;
        while(cx1 != x2 && cy1 != y2){
            if(checkboard.getChecker(cx1,cy1) != Checker::empty){
                jumpedOverCheckersAfterNextJump.push_back(sf::Vector2i(cx1,cy1));
            }
            cx1 += step.x;
            cy1 += step.y;
        }
        bool pawn = isPawn(checkboard.getChecker(x1,y1));
        if(jumpingCheckerStartPos != sf::Vector2i(-1,-1)){
            x1 = jumpingCheckerStartPos.x;
            y1 = jumpingCheckerStartPos.y;
        }
        return mjsf.getMaxiSequenceAfter(x2,y2,x1,y1, pawn,jumpedOverCheckersAfterNextJump);
    }
    int getMaxiJumpingSequenceLenghtFromAll(){
        int res = 0;
        for(int i=0;i<checkboard.getWidth();i++){
            for(int j=0;j<checkboard.getHeight();j++)
                if(isMyChecker(whiteOnTurn,getChecker(i,j)))
                    res = max(res,getMaxiJumpingSequenceLenght(i,j));
        }
        return res;
    }

    int countOfJumpedOverCheckers(){
        return checkboard.jumpedOverCheckers.size();
    }
    bool isWhiteOnTurn(){
        return whiteOnTurn;
    }
    bool isBlackOnTurn(){
        return !whiteOnTurn;
    }
    int getPawnCount(bool white){
        int res = 0;
        for(auto a : checkboard.getBoard()){
            for(auto checker : a){
                if(isMyChecker(white,checker))
                    res++;
            }
        }
        return res;
    }

    std::vector<Checker> getMoveCheckersType() const{
        return moveCheckersType;
    }
    std::vector<int> getCheckerCountVector() const{
        return checkerCountAfterMoves;
    }

    Checker getChecker(int x,int y){
        return checkboard.getChecker(x,y);
    }
    bool isBlockedMovingUp(int x,int y);
    bool isBlockedMovingDown(int x,int y);
    bool isBlockedMoving(int x,int y);
    bool areAllCheckersBlocked(){
        if(getMaxiJumpingSequenceLenghtFromAll() > 0)
            return false;
        for(int i=0;i<checkboard.getWidth();i++){
            for(int j=0;j<checkboard.getHeight();j++){
                if(isMyChecker(whiteOnTurn,getChecker(i,j))){
                    if(isPawn(getChecker(i,j))){
                        if(whiteOnTurn && isBlockedMovingUp(i,j) == false)
                            return false;
                        if(whiteOnTurn == false && isBlockedMovingDown(i,j) == false)
                            return false;
                    }
                    else
                        if(isBlockedMoving(i,j) == false)
                            return false;
                }
            }
        }

        return true;
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

    bool isLongMoving(int x1,int y1,int x2,int y2);
    bool isLongMoving(Quad quad){
        return isLongMoving(quad.x1, quad.y1, quad.x2, quad.y2);
    }

    bool isLongJumping(int x1,int y1,int x2,int y2);
    bool isLongJumping(Quad quad){
        return isLongJumping(quad.x1, quad.y1, quad.x2, quad.y2);
    }

    void move(int x1,int y1,int x2,int y2);

};


class Rules{
    int end_game_state = -1; // -1 -> still playing, 0 - white wins, 1 - black wins, 2 - draw
    int maximumQueenMovesWithoutDraw = 15;
public:
    MoveController & moveController;
    Rules(MoveController& mv) : moveController(mv) {}
    bool whiteWon(){
        return end_game_state == 0;
    }
    bool blackWon(){
        return end_game_state == 1;
    }
    bool isDraw(){
        return end_game_state == 2;
    }
    bool areStillPlaying(){
        return end_game_state == -1;
    }
    virtual bool existAnyCorrectMoveWith(int x1,int y1){
        return true;
    }
    virtual bool isEndOfGame(){
        int blackCheckers = moveController.getPawnCount(false), whiteCheckers = moveController.getPawnCount(true);
        assert(blackCheckers > 0 || whiteCheckers > 0);
        if(blackCheckers == 0)
            end_game_state = 0;
        if(whiteCheckers == 0)
            end_game_state = 1;
        if(moveController.areAllCheckersBlocked()){
            if(moveController.isBlackOnTurn())
                end_game_state = 0;
            else
                end_game_state = 1;
        }

        auto lastMovedCheckers = moveController.getMoveCheckersType();

        if(end_game_state == -1 && lastMovedCheckers.size() >= 2*maximumQueenMovesWithoutDraw){
            bool draw = true;
            for(int i=0;i<maximumQueenMovesWithoutDraw*2;i++){
                if(isPawn(lastMovedCheckers[lastMovedCheckers.size()-1-i])){
                    draw = false;
                    break;
                }
            }
            auto countVect =  moveController.getCheckerCountVector();
            if(countVect.back() == countVect[countVect.size()-1-2*maximumQueenMovesWithoutDraw] && draw == true)
                end_game_state = 2;
        }


        return end_game_state >= 0;
    }
    virtual bool isCorrectWhitePawnMove(int x1, int y1,int x2, int y2) = 0;
    virtual bool isCorrectWhitePawnMove(Quad q) {return isCorrectWhitePawnMove(q.x1,q.y1,q.x2,q.y2);}

    virtual bool isCorrectBlackPawnMove(int x1, int y1,int x2, int y2) = 0;
    virtual bool isCorrectBlackPawnMove(Quad q) {return  isCorrectBlackPawnMove(q.x1,q.y1,q.x2,q.y2);}

    virtual bool isCorrectQueenMove(int x1, int y1,int x2, int y2) = 0;
    virtual bool isCorrectQueenMove(Quad q) {return isCorrectBlackPawnMove(q.x1,q.y1,q.x2,q.y2);}

    virtual bool isCorrectPawnMove(bool isWhite,int x1, int y1,int x2, int y2){
        if(isWhite)
            return isCorrectWhitePawnMove(x1,y1,x2,y2);
        return isCorrectBlackPawnMove(x1,y1,x2,y2);
    }
    virtual bool isCorrectPawnMove(bool isWhite,Quad q){
        if(isWhite)
            return isCorrectWhitePawnMove(q);
        return isCorrectBlackPawnMove(q);
    }
    virtual bool isCorrectMove(int x1,int y1,int x2,int y2){
        Checker checker = moveController.getChecker(x1,y1);
        if(checker == Checker::empty){
            std::cout<<"Warning: Trying to check correctness of empty checker"<<std::endl;
            return false;
        }
        if(checker == Checker::black_pawn){
            if(moveController.isWhiteOnTurn() == true)
                std::cout<<"Warning: Trying to check correctness of black pawn move while current player is white"<<std::endl;
            return isCorrectBlackPawnMove(x1,y1,x2,y2);
        }
        if(checker == Checker::white_pawn){
            if(moveController.isBlackOnTurn() == true)
                std::cout<<"Warning: Trying to check correctness of white pawn move while current player is black"<<std::endl;
            return isCorrectWhitePawnMove(x1,y1,x2,y2);
        }
        if(checker == Checker::white_queen || checker == Checker::black_queen){
            return isCorrectQueenMove(x1,y1,x2,y2);
        }
        throw NotImplementedException();
    }
    virtual bool isCorrectMove(Quad q) {return isCorrectMove(q.x1,q.y1,q.x2,q.y2);}
    virtual ~Rules(){}
};

class ClassicRules : public Rules{
public:
    ClassicRules(MoveController& mv) : Rules(mv) {}
    virtual bool existAnyCorrectMoveWith(int x1,int y1) override{
       return moveController.getMaxiJumpingSequenceLenght(x1,y1) == moveController.getMaxiJumpingSequenceLenghtFromAll();
    }

    virtual bool isCorrectWhitePawnMove(int x1,int y1,int x2,int y2) override{
        if(moveController.countOfJumpedOverCheckers() == 0){
            if(moveController.getMaxiJumpingSequenceLenghtFromAll() == 0 && moveController.isSimpleMovingUp(x1,y1,x2,y2))
                return true;
        }
        if(moveController.isSimpleJumping(x1,y1,x2,y2) &&
            moveController.getMaxiJumpingSequenceLenghtAfterJump(x1,y1,x2,y2) + 1 == moveController.getMaxiJumpingSequenceLenght(x1,y1))
            return true;
        return false;
    }

    virtual bool isCorrectBlackPawnMove(int x1,int y1,int x2,int y2) override{
        if(moveController.countOfJumpedOverCheckers() == 0){
            if(moveController.getMaxiJumpingSequenceLenghtFromAll() == 0 && moveController.isSimpleMovingDown(x1,y1,x2,y2))
                return true;
        }
        if(moveController.isSimpleJumping(x1,y1,x2,y2) &&
            moveController.getMaxiJumpingSequenceLenghtAfterJump(x1,y1,x2,y2) + 1 == moveController.getMaxiJumpingSequenceLenght(x1,y1))
            return true;
        return false;
    }

    virtual bool isCorrectQueenMove(int x1, int y1,int x2, int y2) override{
        if(moveController.countOfJumpedOverCheckers() == 0){
            if(moveController.getMaxiJumpingSequenceLenghtFromAll() == 0 && moveController.isLongMoving(x1,y1,x2,y2))
                return true;
        }
        if(moveController.isLongJumping(x1,y1,x2,y2) &&
            moveController.getMaxiJumpingSequenceLenghtAfterJump(x1,y1,x2,y2) + 1 == moveController.getMaxiJumpingSequenceLenght(x1,y1))
            return true;
        return false;
    }
};

#endif // MOVECONTROLLER_H
