#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define stdin (__acrt_iob_func(0))


void gameMode();
void setup();
void playGame();
void renderBoard();
int updateBoard(int num, char player);
bool checkWin(char player, int BOARD);
void setupBoard();


// Gameboard
char BOARD[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

// Backup board for reseting game
char BOARDcpy[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

// index 0 is player 1, index 1 is player 2
int scoreboard[2] = {0, 0};

// X or O
char player1;
char player2;

// tournament or match
char gamemode[20];

// number of rounds for tournament gamemode
int rounds;
int currentRound;


int main() {
    // Sets the gamemode
    gameMode();
    
    // Sets up player's pieces
    setup();

    // Starts game
    playGame();
}


// Sets tge gamemode
void gameMode(){
    printf("What gamemode would you like to play:\n");
    printf("- Tounrament\n");
    printf("- Match\n");
    printf("Gamemode: ");

    scanf("%s", gamemode);

    if (strncmp(gamemode, "tournament", strlen("tournament")) == 0){
        printf("How many rounds: ");
        scanf("%i", &rounds);
    }
}


// Setups player pieces
void setup(){
    printf("Would you like to play as X or O: ");
    scanf(" %c", &player1);
    player1 = toupper(player1);
    printf("Player 1 is: %c\n", player1);  
    currentRound = 0;
}

// Plays the game
void playGame() {
    // Assings player2 the opposite pieces as player1
    if (player1 == 'X') {
        player2 = 'O';
    }
    else {
        player2 = 'X';
    }

    // Win condition
    bool win = false;

    // Query player for desired peices, update and rerender board
    int found = 0;
    char nextPlayer = player2;
    char currentPlayer = player1;
    int totalMoves = 0;
    while (!win) {
        // Renders board and updates it depending on the desired move
        renderBoard();
        char desiredMove;
        printf("%c, where would you like to move: ", currentPlayer);
        scanf("%c", &desiredMove);
        while (desiredMove == '\n') {
            scanf("%c", &desiredMove);
        }
        
        found = updateBoard(desiredMove, currentPlayer);
        totalMoves++;

        // Checks if move won the game
        win = checkWin(currentPlayer, totalMoves);

        // Prints the winner's piece
        if (win) {
            renderBoard(BOARD);
            printf("%c Wins!\n", currentPlayer);
        }

        // Updates which player is next up
        if (nextPlayer == player1 && found == 0) {
            nextPlayer = player2;
            currentPlayer = player1;
        }
        else if(nextPlayer == player2 && found == 0) {
            nextPlayer = player1;
            currentPlayer = player2;
        }
    }
    
    // Sets up a new game
    if (win) {
        if (strncmp(gamemode, "tournament", strlen("tournament")) == 0)
        {
            currentRound++;
            if (rounds > currentRound){
                setupBoard();
                win = false;
                if (player1 == nextPlayer) {
                    scoreboard[0]++;
                }
                else {
                    scoreboard[1]++;
                }
                printf("Player 1: %i \n", scoreboard[0]);
                printf("Player 2: %i \n", scoreboard[1]);
                printf("\n\n\n");
                playGame();
            }
            else{
                printf("%c won the tournament", nextPlayer);
            }
            
        }
        else 
        {
            char vote;
            printf("Would you like to play again? (Y/N)");
            scanf("%s", &vote);
            if (toupper(vote) == 'Y') {
                setupBoard();
                win = false;
                playGame();
            }
        }
    }
}


// Renders the game board to terminal
void renderBoard() {
    printf(" %c | %c | %c \n", BOARD[0], BOARD[1], BOARD[2]);
    printf("---+---+--- \n");
    printf(" %c | %c | %c \n", BOARD[3], BOARD[4], BOARD[5]);
    printf("---+---+--- \n");
    printf(" %c | %c | %c \n", BOARD[6], BOARD[7], BOARD[8]);
}


// Updates game board with the move
int updateBoard(int num, char player) {
    // Searchs that row for the desired grid piece
    for (int i = 0; i < 9; i++) {
        // Once found replaces it with player's piece
        if (BOARD[i] == num) {
            BOARD[i] = player;
            return 0;
        }
    }
    printf("No Piece Found\n");
    return 1;
}


// checks if player has won
bool checkWin(char player, int moves) {
    bool win = false;

    // Checks if win is possible with number of moves made
    if (moves >= 5) {
        // Checks for horizontal win condition
        for (int i = 0; i < 9 && win == false; i += 3) {
            if (BOARD[i] == player && BOARD[i + 1] == player && BOARD[i + 2] == player) {
                win = true;
                
            }
            else {
                win = false;
            }
        }

        // Checks for vertical win condition
        for (int i = 0; i < 3 && win == false; i++) {
            if (BOARD[i] == player && BOARD[i + 3] == player && BOARD[i + 6] == player) {
                win = true;
                
            }
            else {
                win = false;
            }
        }


        // Checks for negative sloping diagonal win condition
        if (win == false) {
            char winSequence[3] = {player, player, player}; // winSequence -> {player, player, player}

            char playedSequence[3]; // Will be filled with the values of the board
            int position = 0;
            for (int i = 0; i < 3; i++) {
                playedSequence[i] = BOARD[position];
                position += 4;
            }
            if (winSequence[0] == playedSequence[0] && winSequence[1] == playedSequence[1] && winSequence[2] == playedSequence[2]) {
                win = true;
            }    
        }
        
        
        
        // Checks for positive sloping diagonal win condition
        if (win == false){
            char winSequence[3] = {player, player, player}; // winSequence -> {player, player, player}

            char playedSequence[3]; // Will be filled with the values of the board
            int position = 6;
            for (int i = 0; i < 3; i++) {
                playedSequence[i] = BOARD[position];
                position -= 2;
            }
            if (winSequence[0] == playedSequence[0] && winSequence[1] == playedSequence[1] && winSequence[2] == playedSequence[2]) {
                win = true;
            }    
        }
    }
    return win;
}


// Sets up a new game board
void setupBoard(){
    // Resets the pieces on the board
    memcpy(BOARD, BOARDcpy, sizeof BOARD);
}