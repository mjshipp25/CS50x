#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void playGame();
void renderBoard();
int updateBoard(int num, char player);
bool checkWin(char player, int BOARD);
void setup();


// Gameboard
char BOARD[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

// Backup board for resetin game
char BOARDcpy[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};



// index 0 is player 1, index 1 is player 2
int scoreboard[2] = {0, 0};


// X or O
char player1;
char player2;


int main()
{
    // Assigns the players pieces
    printf("Would you like to play as X or O: ");
    scanf("%c", &player1);
    player1 = toupper(player1);
    printf("Player 1 is: %c\n", player1);
    
    // Starts game
    playGame();
}


// Plays the game
void playGame()
{
    // Assings player2 the opposite pieces as player1
    if (player1 == 'X')
    {
        player2 = 'O';
    }
    else
    {
        player2 = 'X';
    }

    // Win condition
    bool win = false;

    // Query player for desired peices, update and rerender board
    int found = 0;
    char nextPlayer = player1;
    int totalMoves = 0;
    while (!win)
    {
        // Renders board and updates it depending on the desired move
        renderBoard();
        char desiredMove;
        printf("%c, where would you like to move: ", nextPlayer);
        scanf("%c", &desiredMove);
        while (desiredMove == '\n')
        {
            scanf("%c", &desiredMove);
        }
        
        found = updateBoard(desiredMove, nextPlayer);
        totalMoves++;

        // Checks if move won the game
        win = checkWin(nextPlayer, totalMoves);

        // Prints the winner's piece
        if (win)
        {
            renderBoard(BOARD);
            printf("%c Wins!\n", nextPlayer);
        }

        // Updates which player is next up
        if (nextPlayer == player1 && found == 0)
        {
            nextPlayer = player2;
        }
        else if(nextPlayer == player2 && found == 0)
        {
            nextPlayer = player1;
        }
    }
    
    // Sets up a new game
    if (win)
    {
        char vote;
        printf("Would you like to play again? (Y/N)");
        scanf("%s", &vote);
        if (toupper(vote) == 'Y')
        {
            setup();
            win = false;
            if (player1 != nextPlayer)
            {
                scoreboard[0]++;
            }
            else
            {
                scoreboard[1]++;
            }
            printf("Player 1: %i \n", scoreboard[0]);
            printf("Player 2: %i \n", scoreboard[1]);
            playGame();
        }
    }
}


// Renders the game board to terminal
void renderBoard()
{
    printf(" %c | %c | %c \n", BOARD[0], BOARD[1], BOARD[2]);
    printf("---+---+--- \n");
    printf(" %c | %c | %c \n", BOARD[3], BOARD[4], BOARD[5]);
    printf("---+---+--- \n");
    printf(" %c | %c | %c \n", BOARD[6], BOARD[7], BOARD[8]);
}


// Updates game board with the move
int updateBoard(int num, char player)
{
    // Searchs that row for the desired grid piece
    for (int i = 0; i < 9; i++)
    {
        // Once found replaces it with player's piece
        if (BOARD[i] == num)
        {
            BOARD[i] = player;
            return 0;
        }
    }
    printf("No Piece Found\n");
    return 1;
}


// checks if player has won
bool checkWin(char player, int moves)
{
    // Checks if win is possible with number of moves made
    if (moves >= 5)
    {
        // Checks for horizontal win condition
        for (int i = 0; i < 9; i++)
        {
            if ((BOARD[i] != player))
            {
                return false;
                
            }
            else
            {
                return true;
            }
        }

        // Checks for vertical win condition
        for (int i = 0; i < 9; i += 3)
        {
            if ((BOARD[i] != player))
            {
                return false;
                
            }
            else
            {
                return true;
            }
        }

        // Checks for negative sloping diagonal win condition
        for (int i = 0; i < 9; i += 4)
        {
            if (BOARD[i] != player)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        
        // Checks for positive sloping diagonal win condition
        for (int i = 8; i >= 0; i -= 4)
        {
            if (BOARD[i] != player)
            {
                return false;
            }
            else
            {
                return true;
            }
            
        }
    }
    return false;
}


// Sets up a new game board
void setup()
{
    // Resets the pieces on the board
    memcpy(BOARD, BOARDcpy, sizeof BOARD);
}
