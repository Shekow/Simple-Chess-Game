#include "ChessBoard.hpp"

int dx__[] = {0, 1, 0, -1, 1, 1, -1, -1};
int dy__[] = {1, 0, -1, 0, 1, -1, 1, -1};

bool ChessBoard::inBounds(int x, int y)
{
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

pair<size_t,size_t> ChessBoard::getPI(string s) 
{
    return {8 - s[1] + '0', s[0] - 'a'};
}
string ChessBoard::getCI(pair<size_t,size_t> in) 
{
    string s;
    s.push_back(in.second + 'a');
    s.push_back(8 - in.first + '0');
    return s;
}

int ChessBoard::colorOfPiece(int x, int y)
{
    if (board[x][y] == '.') return -1;
    return isupper(board[x][y]);
}

bool ChessBoard::isCheck()
{
    pair<size_t, size_t> kingIndex{};
    for (int i{}; i < 8; i++)
    {
        for (int j{}; j < 8; j++)
        {
            if (colorOfPiece(i, j) == turn && toupper(board[i][j]) == 'K')
            {
                kingIndex = {i, j};
            }
        }
    }
    for (int i{}; i < 8; i++)
    {
        for (int j{}; j < 8; j++)
        {
            if (colorOfPiece(i, j) != turn)
            {
                auto moves = potMoves(i, j);
                if (find(all(moves), kingIndex) != moves.end()) return true;
            }   
        }
    }
    return false;
}


vector<pair<size_t, size_t>> ChessBoard::potMoves(size_t x, size_t y)
{
    char p = toupper(board[x][y]);
    bool color = colorOfPiece(x, y);
    vector<pair<size_t, size_t>> moves;
    if (p == 'P')
    {
        int diff = (color ? -1 : 1), i{-1};
        vector<pair<int, int>> tmp = {{x + diff, y}, {x + diff, y + 1}, {x + diff, y - 1}};
        if (color && x == 6 || !color && x == 1) tmp.push_back({x + 2 * diff, y});
        for (each e: tmp)
        {
            i++;
            if (!inBounds(e.first, e.second)) continue;
            auto nc = colorOfPiece(e.first, e.second);
            if (i == 1 || i == 2)
            {
                if (nc == -1 || color == nc) continue;
            }
            else if (nc != -1) continue;
            moves.push_back(e);
            
        }
    }
    else if (p == 'N')
    {
        for (pll ee: vector<pll>{{1, 2}, {-1, 2}, {1, -2}, {-1, -2}})
        {
            for (pll e: {ee, {ee.second, ee.first}})
            {
                e.first += x, e.second += y;
                if (inBounds(e.first, e.second) && color != colorOfPiece(e.first, e.second))
                {
                    moves.push_back(e);
                }
            }
        }
    }
    else if (p == 'B' || p == 'Q')
    {
        for (int i{4}; i < 8; i++)
        {
            for (int nx = x + dx__[i], ny = y + dy__[i]; inBounds(nx, ny); nx += dx__[i], ny += dy__[i])
            {
                if (board[nx][ny] != '.')
                {
                    if (color != colorOfPiece(nx, ny))
                    {
                        moves.push_back({nx, ny});
                    }
                    break;
                }
                moves.push_back({nx, ny});
            }
        }
    }
    if (p == 'R' || p == 'Q')
    {
        for (int i{}; i < 4; i++)
        {
            for (int nx = x + dx__[i], ny = y + dy__[i]; inBounds(nx, ny); nx += dx__[i], ny += dy__[i])
            {
                if (board[nx][ny] != '.')
                {
                    if (color != colorOfPiece(nx, ny))
                    {
                        moves.push_back({nx, ny});
                    }
                    break;
                }
                moves.push_back({nx, ny});
            }
        }
    }
    if (p == 'K')
    {
        for (int i{}; i < 8; i++)
        {
            int nx = x + dx__[i], ny = y + dy__[i];
            if (inBounds(nx, ny) && color != colorOfPiece(nx, ny))
            {
                moves.push_back({nx, ny});
            }
        }
        int r = (color ? 7 : 0);
        if (!HBM[r][4])
        {
            for (each e: {7, 0})
            {
                if (!HBM[e][r])
                {
                    bool freePas{true};
                    int diff = (e == 7 ? 1 : -1);
                    for (int i{4 + diff}; inBounds(r, i); i += diff)
                    {
                        if (board[r][i] != '.') freePas = false;
                    }
                    if (freePas)
                    {
                        moves.push_back({r, e});
                    }   
                }
            }
        }
    }
    return moves;
}

vector<pair<size_t, size_t>> ChessBoard::validMoves(size_t x, size_t y)
{
    vector<pair<size_t, size_t>> vMoves;
    for (each m: potMoves(x, y))
    {
        char victim = board[m.first][m.second];
        board[m.first][m.second] = board[x][y], board[x][y] = '.';
        if (!isCheck()) vMoves.push_back(m);
        board[x][y] = board[m.first][m.second], board[m.first][m.second] = victim;
    }
    return vMoves;
}

bool ChessBoard::isCheckMate() 
{
    return getAllValidMoves().empty() && isCheck();
}

bool ChessBoard::isStaleMate() 
{
    return getAllValidMoves().empty() && !isCheck();
}

bool ChessBoard::playMove(string move) 
{
    for (auto it: getAllValidMoves()) 
    {
        if(move == it)
        {
            pair<size_t,size_t> temp_f = getPI(move.substr(1,2)), temp_t = getPI(move.substr(3,2));
            board[temp_f.first][temp_f.second] = '.';
            board[temp_t.first][temp_t.second] = ((turn == true) ? move[0] : tolower(move[0]));
            turn =! turn;
            HBM[temp_f.first][temp_f.second] = true;
            prevBoards.push_back({board, HBM});
            return true;
        }
    }
    return false;
}

ChessBoard::ChessBoard() 
{
    board = {"rnbqkbnr",
            "pppppppp",
            "........",
            "........",
            "........",
            "........",
            "PPPPPPPP",
            "RNBQKBNR"};
    turn = true;
    HBM = vector<vector<bool>>(8, vector<bool>(8, false));
    prevBoards.push_back({board, HBM});
}

vector<string> ChessBoard::getAllValidMoves() 
{
    vector<string> moves;
    for (int i{}; i < 8; i++) 
    {
        for (int j{}; j < 8; j++)
        {
            if(colorOfPiece(i, j) == turn)
            {
                for (each it: validMoves(i,j)) 
                {
                    string move = char(toupper(board[i][j])) + getCI({i, j}) + getCI(it);
                    moves.push_back(move);
                }
            }
        }
    }
    return moves;
}


void ChessBoard::displayBoard()
{
   const wchar_t cp[][2] = {L"\u2654", L"\u2655", L"\u2656", L"\u2657", L"\u2658", L"\u2659"
   , L"\u265A", L"\u265B", L"\u265C", L"\u265D", L"\u265E", L"\u265F"};
   string en = "KQRBNPkqrbnp";
    for (int i{}; i < 8; i++)
    {
        cout << 8 - i << ' ';
        for (int j{}; j < 8; j++)
        {
            if (board[i][j] == '.') cout << ".";
            else
            {
                HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
                DWORD written = 0;
                WriteConsoleW(handle, cp[en.find(board[i][j])], 1, &written, NULL);
            }
            
            cout << ' ';
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

int ChessBoard::getWinner()
{
    if (isCheckMate()) return !turn;
    return -1;
}

bool ChessBoard::undoLastMove()
{
    if (prevBoards.size() == 1) return false;
    prevBoards.pop_back();
    board = prevBoards.back().first;
    HBM = prevBoards.back().second;
    turn = !turn;
    return true;
}

tuple<double, string> AI::evaluatePos(bool turn, int dep, int lastBest)
{
    double res = (turn == color ? -10 : 10), nextEva;
    string bestMove;
    if (board.isCheckMate())
    {
        if (board.getWinner() == color) res = 1;
        res = -1;
    }
    else if (board.isStaleMate() || !dep) res = 0;
    else
    {    
        auto moves = board.getAllValidMoves();
        for (auto &m: moves)
        {
            board.playMove(m);
            nextEva = get<0>(evaluatePos(!turn, dep - 1, res));
            if ((color == turn && res < nextEva) || (color != turn && res > nextEva))
            {
                res = nextEva, bestMove = m;
            }
            board.undoLastMove()
            if ((color == turn && (res == 1 || res > lastBest)) || (color != turn && (res == -1 || res < lastBest)))
            {
                break;
            }
        }
        
    }
    return {res, bestMove};
}
AI::AI(ChessBoard &board, bool color): board(board), color(color){}
string AI::getMove()
{
    return get<1>(evaluatePos(color));
}