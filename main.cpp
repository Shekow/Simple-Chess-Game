#include <iostream>
#include <bits/stdc++.h>
#include "ChessBoard.cpp"
#define ull unsigned long long
#define ll long long
#define ld long double
#define each auto &
#define endl '\n'
#define pll pair<ll, ll>
#define all(v) v.begin(), v.end()
#define INF LONG_LONG_MAX
#define Outsiders freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#define FastAsFBoy ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
using namespace std;

int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};


int main() 
{
    srand(time(NULL));
    ChessBoard board;
    bool whiteTurn = 1;
    AI ai(board, 0);
    while (!board.isCheckMate() && !board.isStaleMate())
    {
        board.displayBoard();
        string in;
        do
        {
            cout << (whiteTurn ? "White"s : "Black"s) << "\'s move: ";
            if (!whiteTurn)
            {
                // auto moves = board.getAllValidMoves();
                // in = moves[rand() % moves.size()];
                in = ai.getMove();
            }
            else cin >> in;
        }
        while (!board.playMove(in));
        whiteTurn = !whiteTurn;
        system("cls");
    }
    board.displayBoard();
    cout << (board.isCheckMate() ? "CheckMate!" : "StaleMate!");
}
