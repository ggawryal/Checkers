#include "MaxiJumpSequenceFinder.h"

void MaxiJumpSequenceFinder::solvePawn(PawnState state,int res){
    maxi = std::max(maxi,res);
    if(dp.count(state) != 0)
        return;
    dp[state] = res;
    std::vector<std::pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        if(canJumpOver(state.x+ex,state.y+ey,ex,ey)){
            PawnState newState = state;
            newState.x += ex*2;
            newState.y += ey*2;
            newState.pawnMask |= (1<<pawnPositionToId[state.x+ex][state.y+ey]);
            bool queen = (board[state.x+ex][state.y+ey] == 4);
            board[state.x+ex][state.y+ey] = 5;

            solvePawn(newState,res+1);

            if(queen)
                board[state.x+ex][state.y+ey] = 4;
            else
                board[state.x+ex][state.y+ey] = 3;
        }
    }
}

void MaxiJumpSequenceFinder::solveQueen(PawnState state,int res){
    maxi = std::max(maxi,res);
    if(dp.count(state) != 0)
        return;
    dp[state] = res;
    std::vector<std::pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        int j = 1;
        while(isInBoard(state.x + j*ex, state.y + j*ey) && board[state.x + j*ex][state.y + j*ey] == 0)
            j++;

        if(isInBoard(state.x + j*ex, state.y + j*ey) && (board[state.x + j*ex][state.y + j*ey] == 3 || board[state.x + j*ex][state.y + j*ey] == 4)){
            int j2 = j+1;
            while(isInBoard(state.x + j2*ex, state.y + j2*ey) && board[state.x + j2*ex][state.y + j2*ey] == 0){
                PawnState newState = state;
                newState.x += j2*ex;
                newState.y += j2*ey;
                newState.pawnMask |= (1<<pawnPositionToId[state.x+j*ex][state.y+j*ey]);

                bool queen = (board[state.x+j*ex][state.y+j*ey] == 4);
                board[state.x + j*ex][state.y + j*ey] = 5;

                solveQueen(newState,res+1);

                if(queen)
                    board[state.x + j*ex][state.y + j*ey] = 4;
                else
                    board[state.x + j*ex][state.y + j*ey] = 3;
                j2++;
            }
        }

    }
}

