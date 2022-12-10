/*
1. AI engine:
    + state presentation: vector<vector<char>>
    + checking if game is end? 
    + detect winer
    + render (text screen)
    + valid moves 
        - detect current player 
        - calculate valid moves 
    + calculate state score
        - final state score 
        - not final state score 
    + calculate best move
2. User interface 
    + asking x or o 
    + input move cordinate 
    + print machine'move 
*/

#include <bits/stdc++.h> 
using namespace std; 

// state presentation
typedef vector<vector<char> > Board; 
typedef pair<int, int> pii;
typedef pair<int, Board> pib;
const int sz = 3;
Board board = Board(sz, vector<char>(sz, ' '));
char human, curr;

//check row, column, diagonal
//check if game over and calculate the final state's score
int checkBoard(Board b)
{
    // check row & column
    for (int i = 0; i < sz; i++)
    {
        if (b[i][0] == b[i][1] && b[i][1] == b[i][2] && b[i][1] != ' ')
            return (b[i][1] == 'X' ? 1 : -1);
        if (b[0][i] == b[1][i] && b[1][i] == b[2][i] && b[1][i] != ' ')
            return (b[1][i] == 'X' ? 1 : -1);
    }
    // check diagonal
    if (b[0][0] == b[1][1] && b[0][0] == b[2][2] && b[0][0] != ' ')
        return (b[1][1] == 'X' ? 1 : -1);
    if (b[2][0] == b[1][1] && b[2][0] == b[0][2] && b[1][1] != ' ')
        return (b[1][1] == 'X' ? 1 : -1);
    // check if draw
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
        {
            if (b[i][j] == ' ')
                return 2;
        }
    }
    return 0;
}


//display the game
void display(Board b)
{
    system("cls");
    cout << "-------------"<<endl;
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
        {
            cout << "| " << b[i][j] << " "; 
        }
        cout << "|" <<endl;
        cout << "-------------"<<endl;
    }
}

// detect next player
char nextPlayer(Board b)
{
	// we count the number of 'X' and 'O'
    int countX = 0, countO = 0;
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if (b[i][j] == 'X') countX++;
            else if (b[i][j] == 'O') countO++;
        }
    }
    if (countX == countO) return 'X';
    else return 'O';
}

// Copy a new board from the old one
Board copy(Board b)
{
    Board a = Board(3, vector<char>(3, ' '));
    for (int i = 0; i < sz; i++){
        for (int j = 0; j < sz; j++){
            a[i][j] = b[i][j];
        }
    }
    return a; 
}

// Find valid moves and put them into a list
vector<Board> validMoves(Board b)
{
    vector<Board> result;
    char player = nextPlayer(b);
    for (int i = 0; i < sz; i++){
        for (int j = 0; j < sz; j++){
            if (b[i][j] == ' '){
                Board NewBoard = copy(b);
                NewBoard[i][j] = player;
                result.push_back(NewBoard);
            }
        }
    }
    return result;
}

// Calculate the score of the current state, use Minimax algorithm
// Minimax algorithm will calculate the worst case then find the best move to counter the situation.
pib getScore(Board b) 
{
	// If the current state is the final state
    if (checkBoard(b) != 2)
    	return make_pair(checkBoard(b), b);

    char player = nextPlayer(b); 
    vector<Board> boards = validMoves(b); 
    int bestScore = 100; 
    Board bestState;
    for (int i = 0; i < boards.size(); i++){
    	Board nb = boards[i];
        pair<int, Board> p = getScore(nb); 
        int score = p.first;
        // if the current player is X, we will maximize the score
        if (player == 'X') {
            if (bestScore < score || bestScore == 100) {
                bestScore = score;
                bestState = nb;
            }
            if (bestScore == 1)	break;		// because the maximum score can be better than 1, so we can stop if bestScore == 1
        } 
		else // otherwise, if the current player is O, we will minimize the score
		{
            if (bestScore > score || bestScore == 100) {
                bestScore = score;
                bestState = nb;
            }
            if (bestScore == -1) break;		// because the minimum score can be less than -1, so we can stop if bestScore == -1
        }
    }
    return make_pair(bestScore, bestState);	// return the bestscore - state of that score (maximum if currPlayer is 'X' and minimun if currPlayer is 'O')
}

// Find the best move for AI
pii getBestMove(Board b)
{
    pib p = getScore(b);
    int score = p.first;
    Board newBoard = p.second;
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if (newBoard[i][j] != b[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}


int main ()
{
    display(board);
    cout << "You want to X/O: ";    cin >> human;
    while (true)
    {
    	curr = nextPlayer(board);
        int x, y;
        if (human == curr)
        {
            replay:
                cout << "Enter the position: ";	cin >> x >> y;
                x--;y--;
                if (board[x][y] != ' ')
                {
                    cout << "your position has been chosen, please choose other position!" << endl;
                    goto replay;
                }
        }
        else
        {
        	pii p = getBestMove(board);
        	x = p.first; y = p.second;
		}
        board[x][y] = curr;
        display(board);
        if (checkBoard(board) != 2)
	    {
	        if (checkBoard(board) == 0)
	        {
	            cout << "draw";
	            return 0;
	        }
			cout << (nextPlayer(board) == human ? "AI WIN" : "HUMAN WIN");
	        return 0;
	    }
        
    }
}
