#include <iostream>
#include <vector>
#include <map>
using namespace std;

/*
board
  <--> m
/\
|
|
\/
n

0 - empty
1 - white pawn
2 - white queen
3 - balck pawn
4 - balck queen
*5 - bloced
white on move
*/

int n,m;
vector<vector<int> > board;
vector<vector<int> > pawnPositionToId;

struct State{
    int x,y;
    int pawnMask;
    bool amIqueen;
    friend bool operator<(const State& a, const State& b){
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

map<State,int> dp;
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


void solvePawn(State state,int res){
    maxi = max(maxi,res);
    if(dp.count(state) != 0)
        return;

    dp[state] = res;
    vector<pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;
        if(canJumpOver(state.x+ex,state.y+ey,ex,ey)){
            State newState = state;
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

void solveQueen(State state,int res){
    maxi = max(maxi,res);
    if(dp.count(state) != 0)
        return;
    dp[state] = res;
    vector<pair<int,int> > cells = {{1,1},{-1,1},{-1,-1},{1,-1}};
    for(int i=0;i<cells.size();i++){
        int ex = cells[i].first, ey = cells[i].second;

        int j = 1;
        while(isInBoard(state.x + j*ex, state.y + j*ey) && board[state.x + j*ex][state.y + j*ey] == 0)
            j++;

        if(isInBoard(state.x + j*ex, state.y + j*ey) && (board[state.x + j*ex][state.y + j*ey] == 3 || board[state.x + j*ex][state.y + j*ey] == 4)){
            int j2 = j+1;
            while(isInBoard(state.x + j2*ex, state.y + j2*ey) && board[state.x + j2*ex][state.y + j2*ey] == 0){
                State newState = state;
                newState.x += j2*ex;
                newState.y += j2*ey;
                //cout<<newState.x<<" "<<newState.y<<endl;
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



int main(){
    cin >> n >> m;
    board.resize(n,vector<int>(m));
    pawnPositionToId.resize(n,vector<int>(m));
    int blackFigures = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin >> board[i][j];
            if(board[i][j] == 3 || board[i][j] == 4)
                pawnPositionToId[i][j] = blackFigures++;
            else
                pawnPositionToId[i][j] = -1;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(board[i][j] != 1 && board[i][j] != 2){
                cout<<"- ";
                continue;
            }
            dp.clear();
            maxi = 0;
            if(board[i][j] == 1){
                board[i][j] = 0;
                State state;
                state.amIqueen = false;
                state.pawnMask = 0; state.x = i; state.y = j;
                solvePawn(state,0);
                cout<<maxi<<" ";
                board[i][j] = 1;
            }
            else{
                board[i][j] = 0;
                State state;
                state.amIqueen = true;
                state.pawnMask = 0; state.x = i; state.y = j;
                solveQueen(state,0);
                cout<<maxi<<" ";
                board[i][j] = 2;
            }
            //system("PAUSE");
        }
        cout<<endl;
    }
}
/*5 5
1 0 1 0 0
0 3 0 3 0
0 0 0 0 0
0 4 0 3 0
0 0 0 0 1*/

/*8 8
1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0

0 0 0 0 0
0 4 0 3 0
0 0 0 0 1*/
