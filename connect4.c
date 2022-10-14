#include <stdio.h>
#include "./headerFiles/header.h"
#include <time.h>

#define FOUR 4
#define ROWS 6
#define COLUMNS 7

char board[ROWS][COLUMNS];
char header[] = "-----------------------------";
char player1[30];
char player2[30];
clock_t player1Time=0;
clock_t player2Time=0;
char color;

int main()
{
    printf("\n Please enter your name player 1: ");
    scanf_s("%s", &player1);
    printf("\n Please enter your name player 2: ");
    scanf_s("%s", &player2);
    printf("\n %s is Heads %s is Tales \n",player1,player2);
    printf("Tossing coin ....\n");
    int toss = time(0) % 2;
    if(toss ==0){ // decides who gets to play first.
        printf("Heads: %s starts\n",player1);
        color = '1';
    } 
    else{
        printf("Tales: %s starts\n",player2);
        color = '2';
    } 
    init_board();
    printBoard();

    while (!checkFull())
    {
        printf("Player %s, your turn!\n", (color == '1') ? player1 : player2);
        clock_t before = clock();
        choose();
        clock_t diff = clock() - before; // records time taken by the player.
        if(color == '1') player1Time += diff; // adds time taken.
        else player2Time += diff;
        printf("\n\n");
        printBoard();
        if (check()) // checks if a player has won or not.
        {
            printf("\n Player  %s wins! \n", (color == '1') ? player1 : player2);
            break;
        }
        Color(); // switches colors for next turn.
    }
    if(checkFull()){
        printf("%s won because his moves was faster.",(player1Time > player2Time)? player2 : player1);
    }

    return 0;
}

void Color()
{
    color = (color == '1') ? '2' : '1'; // switches between colors for each round.
}

void init_board()
{
    // fill the board with the empty character:
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            board[i][j] = '0';
        }
    }
}
//Requires: Nothing 
//Effects: prints out the table formatted.
void printBoard()
{
    // print the board and the board according to current game:
    printf("%s\n", header);

    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            printf("| %c ", board[i][j]);
            
        }
        printf("|\n");
    }
    printf("%s\n", header);
}

void choose()
{
    char c;
    int column = -1;
    while (column == -1) // as long as we haven't found an empty square yet.
    {
        printf("\nChoose column: ");
        scanf_s(" %c", &c);
        switch (c) // Switch case used to avoid unwanted inputs from being used.
        {
        case '1':
            column = 0;
            break;
        case '2':
            column = 1;
            break;
        case '3':
            column = 2;
            break;
        case '4':
            column = 3;
            break;
        case '5':
            column = 4;
            break;
        case '6':
            column = 5;
            break;
        case '7':
            column = 6;
            break;
        default:
            printf("\nWrong column number\n"); // if not any of the options given, then the input is unwanted/unusable.
            printBoard();
        }
        column = (fill_bin(column) == 1) ? 1 : -1;
    }
}

// Requires: The number of the column the player wants to insert to. The number should be between 1 and 7.
// Effects: Fills out the board where it is required.
int fill_bin(int column)
{
    int fail = -1;
    if (column == -1)
        return fail;
    int level;

    for (level = ROWS - 1; level >= 0; level--)
    {
        if (board[level][column] == '0') // checks for an empty square from the bottom up in the column.
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
// Requires: Nothing.
// Effects: Checks if the winning condition is satisfied vertically.
int checkVertical()
{
    int i, j;
    int count = 0;

    for (j = 0; j < COLUMNS; j++)
    {
        for (i = ROWS - 1; i >= 0; i--)
        {
            if (count + i < FOUR) // if ROWS is less than 4, then no one can win vertically.
                return 0;
            if (count == FOUR)
                return 1;
            if (board[i][j] == color)
                count++;
            else
                count = 0;
        }
    }
}

// Requires: Nothing.
// Effects: Checks if the winning condition is satisfied horizontally.
int checkHorizontal()
{
    int i, j;
    int count = 0;
    for (i = ROWS - 1; i >= 0; i--)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            if (count + (COLUMNS - j) < FOUR) // If Columns are less than 4, then no one can win horizontally.
                return 0;
            if (count == FOUR)
                return 1;
            if (board[i][j] == color)
                count++;
            else
                count = 0;
        }
    }
}
// Requires: Nothing.
// Effects: Checks if the winning condition is satisfied Obliquely.
int checkOblique() // tests both orientations each in both directions.
{
    int i, j;
    int count;

    int ii, jj;
    for (i = 1; i < ROWS - 1; i++)
    {
        for (j = 1; j < COLUMNS - 1; j++)
        {

            /* left-tilted diagonals */
            count = 0;
            // left-upwards:
            for (ii = i, jj = j; (ii >= 0) || (jj >= 0); ii--, jj--)
            {
                if (board[ii][jj] == color)
                {
                    count++;
                    if (count == FOUR)
                        return 1;
                }
                else
                    break;
            }
            // right-downwards:
            for (ii = i + 1, jj = j + 1; (ii <= ROWS - 1) || (jj <= COLUMNS - 1); ii++, jj++)
            {
                if (board[ii][jj] == color)
                {
                    count++;
                    if (count == FOUR)
                        return 1;
                }
                else
                    break;
            }

            /* right-tilted diagonals */
            count = 0;
            // left-downwards:
            for (ii = i, jj = j; (ii <= ROWS - 1) || (jj >= 0); ii++, jj--)
            {
                if (board[ii][jj] == color)
                {
                    count++;
                    if (count == FOUR)
                        return 1;
                }
                else
                    break;
            }
            // right-upwards:
            for (ii = i - 1, jj = j + 1; (ii >= 0) || (jj <= COLUMNS - 1); ii--, j++)
            {
                if (board[ii][jj] == color)
                {
                    count++;
                    if (count == FOUR)
                        return 1;
                }
                else
                    break;
            }
        }
    }

    return 0;
}
// Requires: Nothing.
// Effects: Check if a player has won.
int check()
{
    return checkHorizontal() || checkVertical() || checkOblique(); // player wins if either of these are satisfied.
}

int checkFull(){
    for(int i = 0; i < ROWS;i++){
        for(int j = 0; j < COLUMNS; j++){
            if(board[i][j] == '0') return 0; // if one square is equal to 0, then the board is not full
        }
    }
    return 1; // all squares are full.
}