#ifndef MAXIJUMPSEQUENCEFINDER_H
#define MAXIJUMPSEQUENCEFINDER_H

#include <iostream>
#include <vector>
#include <map>
#include "Checker.h"

class MaxiJumpSequenceFinder{
    int n,m;
    bool whiteOnTurn = true;
    std::vector<std::vector<Checker> > board;
    std::vector<std::vector<int> > pawnPositionToId;
    struct PawnState{
        int x,y;
        int pawnMask;
        bool amIqueen;
        PawnState(int X=0,int Y=0, int PawnMask = 0,bool AmIqueen = false){
            x = X;
            y = Y;
            pawnMask = PawnMask;
            amIqueen = AmIqueen;

        }
        friend bool operator<(const PawnState& a, const PawnState& b){
            if(a.x == b.x){
                if(a.y == b.y){
                    if(a.pawnMask == b.pawnMask){
                        return a.amIqueen < b.amIqueen;
                    }
                    return a.pawnMask < b.pawnMask;
                }
                return a.y < b.y;
            }
            return a.x < b.x;
        }
    };
    std::map<PawnState,int> dp;
    bool isInBoard(int x,int y){
        return x >= 0 && y >=0 && x<n && y<m;
    }
    bool canJumpOver(int x,int y,int ex,int ey){
        if(isInBoard(x,y) && isEnemyChecker(whiteOnTurn,board[x][y]))
            if(isInBoard(x+ex,y+ey) && board[x+ex][y+ey] == Checker::empty)
               return true;
        return false;
    }
    void solvePawn(PawnState state);
    void solveQueen(PawnState state);
public:
    void loadBoard(std::vector<std::vector<Checker> > b){
        board = b;
        n = board.size();
        if(n == 0)
            m = 0;
        else
            m = board[0].size();
    }
    void reset(){
        dp.clear();
    }
    int solve(int x,int y,bool pawn){
        reset();
        Checker prevChecker = board[x][y];
        board[x][y] = Checker::empty;
        int res = 0;
        if(pawn){
            solvePawn(PawnState(x,y,0,false));
            res = dp[PawnState(x,y,0,false)];
        }
        else{
            solveQueen(PawnState(x,y,0,true));
            res = dp[PawnState(x,y,0,true)];
        }
        board[x][y] = prevChecker;
        return res;

    }



};
#endif // MAXIJUMPSEQUENCEFINDER_H
