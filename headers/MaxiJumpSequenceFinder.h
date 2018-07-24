#ifndef MAXIJUMPSEQUENCEFINDER
#define MAXIJUMPSEQUENCEFINDER

#include <iostream>
#include <vector>
#include <map>

class MaxiJumpSequenceFinder{
    int n,m;
    std::vector<std::vector<int> > board;
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
    int maxi = 0;
    bool isInBoard(int x,int y){
        return x >= 0 && y >=0 && x<n && y<m;
    }
    bool canJumpOver(int x,int y,int ex,int ey){
        if(isInBoard(x,y) && (board[x][y] == 3 || board[x][y] == 4))
            if(isInBoard(x+ex,y+ey) && (board[x+ex][y+ey] == 0))
               return true;
        return false;
    }
    void solvePawn(PawnState state,int res);
    void solveQueen(PawnState state,int res);
public:
    void loadBoard(std::vector<std::vector<int> > b){
        board = b;
        n = board.size();
        if(n == 0)
            m = 0;
        else
            m = board[0].size();
    }
    void reset(){
        maxi = 0;
        dp.clear();
    }
    int solve(int x,int y,bool pawn){
        reset();
        int prevChecker = board[x][y];
        board[x][y] = 0;
        if(pawn)
            solvePawn(PawnState(x,y,0,false),0);
        else
            solveQueen(PawnState(x,y,0,true),0);
        board[x][y] = prevChecker;
        return maxi;
    }



};
#endif // MAXIJUMPSEQUENCEFINDER
