#pragma once
#include <iostream>
#include <bits/stdc++.h>
#include "windows.h"
#define ull unsigned long long
#define ll long long
#define ld long double
#define each auto &
#define endl '\n'
#define pll pair<ll, ll>
#define all(v) v.begin(), v.end()
#define INF LONG_LONG_MAX
using namespace std;



class ChessBoard
{
    vector<string> board;
    vector<vector<bool>> HBM;
    vector<pair<vector<string>, vector<vector<bool>>>> prevBoards;
    bool turn;
    bool inBounds(int x, int y);
    int colorOfPiece(int x, int y);
    pair<size_t,size_t> getPI(string s);
    string getCI(pair<size_t,size_t> in);
    vector<pair<size_t, size_t>> potMoves(size_t x, size_t y);
    vector<pair<size_t, size_t>> validMoves(size_t x, size_t y);
    public:
        ChessBoard();
        void displayBoard();
        vector<string> getAllValidMoves();
        bool playMove(string move);
        bool undoLastMove();
        bool isCheck();
        bool isCheckMate();
        bool isStaleMate();
        int getWinner();
};


class AI
{
    bool color;
    ChessBoard &board;
    tuple<double, string> evaluatePos(bool turn, int dep = 3, int lastBest = 10);
    public:
        AI(ChessBoard &board, bool color = false);
        string getMove();
};