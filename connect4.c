#include <stdio.h>
#include "./headerFiles/header.h"
#include <time.h>
#include <ctype.h>
#include "Bot.h"

#define FOUR 4
#define ROWS 6
#define COLUMNS 7

int** board;
char header[] = "-----------------------------";
char player1[30];
char player2[30];
clock_t player1Time = 0;
clock_t player2Time = 0;
int color;

int main()
{
    printf("\n Please enter your name: ");
    scanf_s("%s", &player1);
    player2[0]='B';
    player2[1]='o';
    player2[2]='t';
    printf("\n %s is Heads %s is Tales \n", player1, player2);
    printf("Tossing coin ....\n");
    int toss = time(0) % 2;
    if(toss ==0){ // decides who gets to play first.
        printf("Heads: %s starts\n",player1);
        color = 1;
    }
    else
    {
        printf("Tales: %s starts\n", player2);
        color = 2;
    }
    init_board();
    printBoard();

    while (!checkIfFull())
    {
        printf("Player %s, your turn!\n", (color == 1) ? player1 : player2);
        clock_t before = clock();
        choose();
        clock_t diff = clock() - before; // records time taken by the player.
        if(color == 1) player1Time += diff; // adds time taken.
        else player2Time += diff;
        printf("\n\n");
        printBoard();
        if (check()) // checks if a player has won or not.
        {
            printf("\n Player  %s wins! \n", (color == 1) ? player1 : player2);
            break;
        }
        Color(); // switch colors for next turn.
    }
    if (checkIfFull())
    {
        printf("%s won because his moves was faster.", (player1Time > player2Time) ? player2 : player1);
    }

    return 0;
}

void Color()
{
    color = (color == 1) ? 2 : 1; // switches between colors for each round.
}

void init_board()
{
    // fill the board with the empty character:
    board = (int**)malloc(ROWS * sizeof(int*));
    for (int i = 0; i < COLUMNS; i++)
        board[i] = (int*)malloc(COLUMNS * sizeof(int));
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
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

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLUMNS; j++)
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
            printf("\nChoose column: ");
            scanf_s("%d", &c);
            if( !isdigit(c) )
                column = c-1;
            else {
                printf("\nWrong column number\n");
                printBoard();
            }
        }
        else column = Minimax(board, 7, 1)[1];

        //checks for filled columns
        column = (fill_bin(column) == 1) ? 1 : -1;

    }
}

/*
    Requires: The number of the column the player wants to insert to. The number should be between 1 and 7.
    Effects: Fills out the board where it is required.
*/
int fill_bin(int column)
{
    int fail = -1;
    if (column == -1) return fail;
    int level;

    for (level = ROWS - 1; level >= 0; level--)
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
   Effects: Checks if the winning condition is satisfied vertically.
*/
//int checkVertical()
//{
//    int i, j;
//    int count = 0;
//
//    for (j = 0; j < COLUMNS; j++)
//    {
//        for (i = ROWS - 1; i >= 0; i--)
//        {
//            if (count + i < FOUR) // if ROWS is less than 4, then no one can win vertically.
//                return 0;
//            if (count == FOUR)
//                return 1;
//            if (board[i][j] == color)
//                count++;
//            else
//                count = 0;
//        }
//    }
//}
//
///*
//    Requires: Nothing
//    Effects: Checks if the winning condition is satisfied horizentally.
//*/
//int checkHorizontal()
//{
//    int i, j;
//    int count = 0;
//    for (i = ROWS - 1; i >= 0; i--)
//    {
//        for (j = 0; j < COLUMNS; j++)
//        {
//            if (count + (COLUMNS - j) < FOUR) // If Columns are less than 4, then no one can win horizontally.
//                return 0;
//            if (count == FOUR)
//                return 1;
//            if (board[i][j] == color)
//                count++;
//            else
//                count = 0;
//        }
//    }
//}
///*
//    Requires: Nothing//
//    Effects: Checks if the winning condition is satisfied Obliquely.
//*/
//int checkOblique()
//{
//    int i, j;
//    int count;
//
//    int ii, jj;
//    for (i = 1; i < ROWS - 1; i++)
//    {
//        for (j = 1; j < COLUMNS - 1; j++)
//        {
//
//            /*
//             left-tilted diagonals
//             */
//            count = 0;
//            // left-upwards:
//            for (ii = i, jj = j; (ii >= 0) || (jj >= 0); ii--, jj--)
//            {
//                if (board[ii][jj] == color)
//                {
//                    count++;
//                    if (count == FOUR)
//                        return 1;
//                }
//                else
//                    break;
//            }
//            // right-downwards:
//            for (ii = i + 1, jj = j + 1; (ii <= ROWS - 1) || (jj <= COLUMNS - 1); ii++, jj++)
//            {
//                if (board[ii][jj] == color)
//                {
//                    count++;
//                    if (count == FOUR)
//                        return 1;
//                }
//                else
//                    break;
//            }
//
//            /*
//              right-tilted diagonals
//             */
//            count = 0;
//            // left-downwards:
//            for (ii = i, jj = j; (ii <= ROWS - 1) || (jj >= 0); ii++, jj--)
//            {
//                if (board[ii][jj] == color)
//                {
//                    count++;
//                    if (count == FOUR)
//                        return 1;
//                }
//                else
//                    break;
//            }
//            // right-upwards:
//            for (ii = i - 1, jj = j + 1; (ii >= 0) || (jj <= COLUMNS - 1); ii--, j++)
//            {
//                if (board[ii][jj] == color)
//                {
//                    count++;
//                    if (count == FOUR)
//                        return 1;
//                }
//                else
//                    break;
//            }
//        }
//    }
//
//    return 0;
//}
int wonCheck( int token){

    //score horizontal.
    for (int i = 0; i < ROWS; ++i) {
        int* row = board[i];
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (row[j+k] == token) count++;
            }
            if(count == FOUR) return 1;
        }
    }
    //score vertical
    for (int i = 0; i < COLUMNS; ++i) {
        for (int j = 0; j < 3; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[j+k][i] == token) count++;
            }
            if(count == FOUR) return 1;
        }
    }
    //positively sloping
    for (int i = 3; i < ROWS; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i-k][j+k] == token) count++;
            }
            if(count == FOUR) return 1;
        }
    }
    //negatively sloping
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i+k][j+k] == token) count++;
            }
            if(count == FOUR) return 1;
        }
    }
    return 0;
}
/*
 Requires: Nothing.
 Effects: Check if a player has won.
*/
 int check()
{
    return wonCheck(color);
//    return checkHorizontal() || checkVertical() || checkOblique(); // player wins if either of these are satisfied.
}
/*
    requires nothing
    checks if the board is full
    board is full --> returns 1
*/
int checkIfFull(){
    for(int i = 0; i < ROWS;i++){
        for(int j = 0; j < COLUMNS; j++){
            if(board[i][j] == 0) return 0; // if one square is equal to 0, then the board is not full
        }
    }
    return 1; // all squares are full.
}