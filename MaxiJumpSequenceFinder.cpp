#include "MaxiJumpSequenceFinder.h"
using namespace std;
void MaxiJumpSequenceFinder::solvePawn(PawnState state){
    if(dp.count(state) > 0)
        return;
    dp[state] = 0;
    //std::cout<<"visited "<< state.x<<" "<<state.y<<" "<<state.sx<<" "<<state.sy<<" "<<state.pawnMask<<" "<<state.amIqueen<< std::endl;
    std::vector<std::pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        if(canJumpOver(state.x+ex,state.y+ey,ex,ey)){
        //    std::cout<<"i can jump over"<<std::endl;
            PawnState newState = state;
            newState.x += ex*2;
            newState.y += ey*2;
         //   cout<<"no w sumie spoko"<<endl;
            newState.pawnMask |= (1<<pawnPositionToId[state.x+ex][state.y+ey]);
         //   cout<<"Krzysztof Pazdro"<<endl;
            Checker backedUp = board[state.x+ex][state.y+ey];
            board[state.x+ex][state.y+ey] = Checker::jumped_over;

            solvePawn(newState);
            dp[state] = std::max(dp[state],dp[newState]+1);
            maxi = std::max(maxi,dp[state]);
            board[state.x+ex][state.y+ey] = backedUp;
        }
    }

}

void MaxiJumpSequenceFinder::solveQueen(PawnState state){

    if(dp.count(state) != 0)
        return;
    std::cout<<"visited "<< state.x<<" "<<state.y<<" "<<state.sx<<" "<<state.sy<<" "<<state.pawnMask<<" "<<state.amIqueen<< std::endl;
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

