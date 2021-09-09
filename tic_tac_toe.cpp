#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Game
{
    public:
        vector<vector<string>> board = {{"P11", "P12", "P13"}, {"P21", "P22", "P23"}, {"P31", "P32", "P33"}};

        // Don't need a constructor or destructor since no dynamic memory
        Game(){}
        ~Game(){}

        // check if the game ends
        bool isEnd(vector<vector<string>> board)
        {
            for (int i = 0; i < 3; i++)
            {
                if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
                if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
                if (i == 0)
                {
                    if (board[i][i] == board[i+1][i+1] && board[i+1][i+1] == board[i+2][i+2]) return true;
                }
            }
            if (board[2][0] == board[1][1] && board[1][1] == board[0][2]) return true;  
            return false;
        }
        // check if the board is full
        bool isFull(vector<vector<string>> board)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j][0] == 'P') return false;
                }
            }
            return true;
        }
        // the player takes a position
        void play(int i, int j, char c)
        {
            board[i][j] = c;
        }
        // print the entire board
        void printBoard(vector<vector<string>> board)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cout << board[i][j] << "  ";
                }
                cout << ' ' << endl;
            }
        }
        // check if the move is a valid position
        bool isValid(int i, int j, vector<vector<string>> board)
        {
            if (i < 0 || i > 2 || j < 0 || j > 2 || board[i][j][0] != 'P') return false;
            return true;
        }

        void player(char playerChar)
        {
            int row;
            int col;
            cout << "It's your turn! Enter the position(separated by a space, for example 1 3): " << endl;
            cin >> row >> col;
            // if the player enters an invalid position
            if (!isValid(row-1, col-1, board))
            {
                cout << "Bad Player! Bad Bad Player! You Lose!" << endl;
                exit(0);
            }
            play(row-1, col-1, playerChar);
            printBoard(board);
            cout << "" << endl;
            // if the game ends at the player's turn
            if (isEnd(board))
            {
                // the player wins, which is impossible
                cout << "Cogratulations! You Win!" << endl;
            }
            // if all positions are taken and the game doesn't end, it means it's a draw
            if (isFull(board))
            {
                cout << "Tie!" << endl;
            }
        }

        void ai(char aiChar)
        {
            cout << "AI is playing..." << endl;
            int maxScore = -10000;
            int bestPos[2];
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    // if the position is available
                    if (board[i][j][0] == 'P')
                    {
                        board[i][j] = aiChar;
                        char playerChar = (aiChar == 'X') ? 'O' : 'X';
                        int score = minimax(board, false, playerChar);
                        board[i][j] = "P" + to_string(i+1) + to_string(j+1);
                        // If that position is the better position
                        if (score > maxScore)
                        {
                            // updates the score
                            maxScore = score;
                            //AI takes that postion
                            bestPos[0] = i;
                            bestPos[1] = j;
                        }
                    }
                }
            }
            board[bestPos[0]][bestPos[1]] = aiChar;
            printBoard(board);
            cout << "" << endl;
            // if the game ends at AI's turn
            if (isEnd(board))
            {
                // the player loses
                cout << "Oops! You Lose!" << endl;
            }
            // if all positions are taken and the game doesn't end, it means it's a draw
            if (isFull(board))
            {
                cout << "Tie!" << endl;
            }
        }

        // AI is the maximizing player
        int minimax(vector<vector<string>> board, bool isMax, char c)
        {
            // AI wins
            if (isEnd(board) && isMax) return -1;
            // human wins
            else if (isEnd(board) && !isMax) return 1;
            // draw
            else if(isFull(board)) return 0;
            // rest of minimax() is essentially the same as ai()
            else
            {
                if (isMax)
                {
                    int maxScore = -10000;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (board[i][j][0] == 'P')
                            {
                                board[i][j] = c;
                                char otherC = (c == 'X') ? 'O' : 'X';
                                int score = minimax(board, false, otherC);
                                board[i][j] = "P" + to_string(i+1) + to_string(j+1);
                                maxScore = max(maxScore, score);
                            }
                        }
                    }
                    return maxScore;
                }
                else
                {
                    int minScore = 10000;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (board[i][j][0] == 'P')
                            {
                                board[i][j] = c;
                                char otherC = (c == 'X') ? 'O' : 'X';
                                int score = minimax(board, true, otherC);
                                board[i][j] = "P" + to_string(i+1) + to_string(j+1);
                                minScore = min(minScore, score);
                            }
                        }
                    }
                    return minScore;
                }
            }
        }
};

int main()
{
    Game* game = new Game;
    cout << "Welcome to Tic Tac Toe! A P on the board means it is an empty position you can take." << endl;
    game->printBoard(game->board);
    char playerChar;
    cout << "Please enter which symbol(X or O) you want to play: " << endl;
    cin >> playerChar;
    // check if the player does not follow the rules
    if (playerChar != 'X' && playerChar != 'O')
    {
        cout << "You must enter either X or O!" <<endl;
        return 0;
    }
    char aiChar = (playerChar == 'X') ? 'O' : 'X';
    bool playerFirst;
    cout << "Do you want to go first(enter 1 for yes and 0 for no): " << endl;
    cin >> playerFirst;
    // check if the player does not follow the rules
    if (playerFirst != 1 && playerFirst != 0)
    {
        cout << "You must enter either 1 or 0!" << endl;
        return 0;
    }
    int turn = 0;
    while (!game->isFull(game->board) && !game->isEnd(game->board))
    {
        if (playerFirst)
        {
            if (turn % 2 == 0)
            {
                // player's turn
                game->player(playerChar);
            }
            else
            {
                // AI's turn
                game->ai(aiChar);
            }
        }
        else
        {
            if (turn % 2 == 0)
            {
                // AI's turn
                game->ai(aiChar);
            }
            else
            {
                // player's turn
                game->player(playerChar);
            }
        }
        turn++;
    }
    delete game;
}