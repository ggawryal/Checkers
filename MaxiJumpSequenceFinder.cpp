#include "MaxiJumpSequenceFinder.h"

void MaxiJumpSequenceFinder::solvePawn(PawnState state){

    if(dp.count(state) != 0)
        return;
    dp[state] = 0;

    std::vector<std::pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        if(canJumpOver(state.x+ex,state.y+ey,ex,ey)){
            PawnState newState = state;
            newState.x += ex*2;
            newState.y += ey*2;
            newState.pawnMask |= (1<<pawnPositionToId[state.x+ex][state.y+ey]);

            Checker backedUp = board[state.x+ex][state.y+ey];
            board[state.x+ex][state.y+ey] = Checker::jumped_over;

            solvePawn(newState);
            dp[state] = std::max(dp[state],dp[newState]+1);

            board[state.x+ex][state.y+ey] = backedUp;
        }
    }

}

void MaxiJumpSequenceFinder::solveQueen(PawnState state){

    if(dp.count(state) != 0)
        return;

    dp[state] = 0;

    std::vector<std::pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        int j = 1;
        while(isInBoard(state.x + j*ex, state.y + j*ey) && board[state.x + j*ex][state.y + j*ey] == Checker::empty)
            j++;

        if(isInBoard(state.x + j*ex, state.y + j*ey) && isEnemyChecker(whiteOnTurn,board[state.x + j*ex][state.y + j*ey])){
            int j2 = j+1;
            while(isInBoard(state.x + j2*ex, state.y + j2*ey) && board[state.x + j2*ex][state.y + j2*ey] == Checker::empty){
                PawnState newState = state;
                newState.x += j2*ex;
                newState.y += j2*ey;
                newState.pawnMask |= (1<<pawnPositionToId[state.x+j*ex][state.y+j*ey]);

                Checker backedUp = board[state.x + j*ex][state.y + j*ey];
                board[state.x + j*ex][state.y + j*ey] = Checker::jumped_over;

                solveQueen(newState);
                dp[state] = std::max(dp[state],dp[newState]+1);

                board[state.x + j*ex][state.y + j*ey] = backedUp;

                j2++;
            }
        }

    }
}

