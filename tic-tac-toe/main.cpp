/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "grid.h"
#include "set.h"
#include "map.h"
using namespace std;

Map<char,char> nextPlayer={{'X','O'},{'O','X'}};

bool gameEnd(Grid<char> &board,GridLocation loc)
{
    return (board[0][0]==board[1][1]&&board[1][1]==board[2][2])
            ||(board[0][2]==board[1][1]&&board[1][1]==board[2][0])
            ||(board[0][loc.col]==board[1][loc.col]&&board[1][loc.col]==board[2][loc.col])
            ||(board[loc.row][0]==board[loc.row][1]&&board[loc.row][1]==board[loc.row][2]);
}

Set<GridLocation> feasibleMoves(Grid<char> &board)
{
    Set<GridLocation> result;
    for (GridLocation loc:board.locations())
    {
        if (isdigit(board[loc]))
            result.add(loc);
    }
    return result;
}

int optimalMove(Grid<char> &board,GridLocation &move,char player)
{
    if (feasibleMoves(board).isEmpty())
    {
        return 0;
    }
    else
    {
        GridLocation tieMove(-1,-1);
        for (GridLocation loc:feasibleMoves(board))
        {
            char backup=board[loc];
            board[loc]=player;
            if (gameEnd(board,loc))
            {
                move=loc;
                board[loc]=backup;
                return 1;
            }
            else
            {
                GridLocation oppoentMove;
                int oppoentValue=optimalMove(board,oppoentMove,nextPlayer[player]);
                if (oppoentValue==-1)
                {
                    move=loc;
                    board[loc]=backup;
                    return 1;
                }
                else if (oppoentValue==0)
                {
                    tieMove=loc;
                }
            }
            board[loc]=backup;
        }
        if (tieMove!=GridLocation(-1,-1))
        {
            move=tieMove;
            return 0;
        }
        else
        {
            move=feasibleMoves(board).first();
            return -1;
        }
    }
}

int main()
{
    Grid<char> board={{'1','2','3'},{'4','5','6'},{'7','8','9'}};
    cout<<board.toString2D()<<endl;
    bool yes=getYesOrNo("Do you want to play first?");
    char player=yes?'O':'X';
    cout<<endl;
    GridLocation move;
    while (!gameEnd(board,move)&&!feasibleMoves(board).isEmpty())
    {
        if (player=='O')
        {
            int moveNumber=getInteger("enter your choice:")-1;
            move={moveNumber/3,moveNumber%3};
            while (!feasibleMoves(board).contains(move))
            {
                moveNumber=getInteger("Invalid move. Enter your choicr again:")-1;
                move={moveNumber/3,moveNumber%3};
            }
            board[move]='O';
        }
        else
        {
            int myValue=optimalMove(board,move,player);
            if (myValue==-1)
            {
                cout<<"It seems I will fail..."<<endl;
            }
            cout<<"My choice is "<<move.row*3+move.col+1<<endl;
            board[move]='X';
        }
        cout<<"The board now looks like: "<<endl;
        cout<<board.toString2D()<<endl;
        player=nextPlayer[player];
        pause(1000);
    }
    if (gameEnd(board,move))
    {
        cout<<(player=='X'?"you win!":"I win!")<<endl;
    }
    else
    {
        cout<<"we tie!";
    }
    return 0;
}
