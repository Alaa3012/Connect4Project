#include <stdio.h>
#include "header.h"
#include <time.h>
#include "Bot.h"
#include <string.h>

int** board;
char header[] = "-----------------------------";
char player[40];
char bot[] = "bot";
clock_t playerTime = 0;
clock_t botTime = 0;
int BotToken_help;
int PlayerToken_help;
int color;
int EAZY = 0;
int depth;
int main(){
    char mode[10];
    char easy[] = "easy";
    char medium[]="medium";
    char hard[] = "hard";
    printf("\n Please enter your name: ");
    scanf_s("%s", &player);
    printf("Which mode do you want to play? easy medium hard\nType: ");
    scanf_s("%s",&mode);
    if (strcmp(mode, easy) == 0) EAZY = 1;
    else if (strcmp(mode, medium) == 0) depth = 2;
    else if (strcmp(mode, hard) ==0) depth = 8;
    printf("\n %s is Heads %s is Tales \n", player, bot);
    printf("Tossing coin ....\n");
    int toss = time(0) % 2;
    if(toss ==0){ // decides who gets to play first.
        printf("Heads: %s starts\n",player);
        PlayerToken_help = 1;
        BotToken_help = 2;
        color = 1;
    }
    else
    {
        printf("Tales: %s starts\n", bot);
        BotToken_help = 1;
        PlayerToken_help = 2;
        color = 1;
    }
    init_board();
    printBoard();

    while (!checkIfFull())
    {
        printf("Player %s, your turn!\n", (color == 1) ? ((BotToken_help == 1) ? bot : player) : (PlayerToken_help == 2) ? player : bot);
        clock_t before = clock();
        choose();
        clock_t diff = clock() - before; // records time taken by the player.
        if(color == 1){
            if (PlayerToken_help == 1) playerTime += diff;
            else botTime += diff;
        }  // adds time taken.
        else{
            if (PlayerToken_help == 2) playerTime += diff;
            else botTime += diff;
        }
        printf("\n\n");
        printBoard();
        if (wonCheck()) // checks if a player has won_help or not.
        {
            printf("\n Player %s wins! \n", (color == 1) ? ((BotToken_help == 1) ? bot : player) : (PlayerToken_help == 2) ? player : bot);
            break;
        }
        Color(); // switch colors for next turn.
    }
    if (checkIfFull())
    {
        printf("%s won_help because his moves was faster.", (playerTime > botTime) ? bot : player);
    }

    return 0;
}
/*
 Requires: Nothing.
 Effects: Switch between turns.
 */
void Color(){
    color = (color == 1) ? 2 : 1; // switches between colors for each round.
}
/*
 Requires: Nothing.
 Effect: initialize the board to 0.
 */
void init_board(){
    // fill the board with the empty character:
    board = (int**)malloc(ROWS_help * sizeof(int*));
    for (int i = 0; i < ROWS_help; i++){
        board[i] = (int*)malloc(COLUMNS_help * sizeof(int));
        for (int j = 0; j < COLUMNS_help; j++)
        {
            board[i][j] = 0;
        }
    }
}
/*
 Requires: Nothing
 Effects: prints out the table formatted.
*/
 void printBoard()
{
    // print the board and the board according to current game:
    printf("%s\n", header);

    int i, j;

    for (i = 0; i < ROWS_help; i++)
    {
        for (j = 0; j < COLUMNS_help; j++)
        {
            printf("| %d ", board[i][j]);
        }
        printf("|\n");
    }
    printf("%s\n", header);
}

/*
    requires(nothing)
    switch cases are used to filter out all unwanted user inputs
    error--> returns -1
    else --> returns column number to write to
*/
void choose()
{
    int c;
    int column = -1;
    while (column == -1) // as long as we haven't found an empty square yet.
    {
        if(color == 1){
            if (PlayerToken_help == 1){
                printf("\nChoose column: ");
                scanf_s("%d", &c);
                while(c<0 || c>7){
                    printf("\nWrong column number\n");
                    printBoard();
                    printf("%s choose column: ", player);
                    scanf_s("%d", &c);
                }
                column = c-1;
//            column = make_move_Help(board, 1, 8);
            }
            else{
                if (EAZY)column = bestMove_help(board, BotToken_help);
                else column = makeMove(board, depth);
            }
        }

        else{
            if (PlayerToken_help == 2){
                printf("\nChoose column: ");
                scanf_s("%d", &c);
                while(c<0 || c>7){
                    printf("\nWrong column number\n");
                    printBoard();
                    printf("%s choose column: ", player);
                    scanf_s("%d", &c);
                }
                column = c-1;
//            column = make_move_Help(board, 1, 8);
            }
            else{
                if (EAZY)column = bestMove_help(board, BotToken_help);
                else column = makeMove(board, depth);
            }
        }
        //checks for filled columns
        column = (fill_bin(column) == 1) ? 1 : -1;

    }
}

/*
    Requires: The number of the column the player wants to insert to. The number should be between 1 and 7.
    Effects: Fills out the board where it is required.
    Testing: try filling in different columns.
*/
int fill_bin(int column)
{
    int fail = -1;
    if (column == -1) return fail;
    int level;

    for (level = ROWS_help - 1; level >= 0; level--)
    {
        if (board[level][column] == 0) // checks for an empty square from the bottom up in the column.
        {
            board[level][column] = color;
            fail = 1;
            break;
        }
    }
    if (fail == -1) // if fail == -1, that means no empty square was found in that column.
        printf("This column is full! Please choose another one");
    return fail;
}
/*
 Requires: Nothing.
 Effects: return 1 if any winning condition is satisfied and 0 otherwise.
 Testing: try horizontal winning
          try vertical winning
          try oblique positive sloping winning
          try oblique negative sloping winning
 */
int wonCheck(){

    //score horizontal.
    for (int i = 0; i < ROWS_help; ++i) {
        int* row = board[i];
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (row[j+k] == color) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //score vertical
    for (int i = 0; i < COLUMNS_help; ++i) {
        for (int j = 0; j < 3; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[j+k][i] == color) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //positively sloping oblique
    for (int i = 3; i < ROWS_help; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i-k][j+k] == color) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //negatively sloping oblique
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i+k][j+k] == color) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    return 0;
}
/*
 Requires: A board.
 Effects: returns 1 if the board os full and 0 otherwise.
 Testing: try a full board
          try a non-full board
*/
int checkIfFull(){
    for(int i = 0; i < ROWS_help; i++){
        for(int j = 0; j < COLUMNS_help; j++){
            if(board[i][j] == 0) return 0; // if one square is equal to 0, then the board is not full
        }
    }
    return 1; // all squares are full.
}