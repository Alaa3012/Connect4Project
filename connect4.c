#include <stdio.h>
#include "./headerFiles/header.h"
#include <time.h>

#define FOUR 4
#define ROWS 6
#define COLOMNS 7

char board[ROWS][COLOMNS];
char header[] = "-----------------------------";
char grids[] = "|  |  |  |  |  |  |  |";
char player1[30];
char player2[30];
clock_t player1Time = 0;
clock_t player2Time = 0;
char color;

int main()
{
    printf("\n Please enter your name player 1: ");
    scanf("%s", &player1);
    printf("\n Please enter your name player 2: ");
    scanf("%s", &player2);
    printf("\n %s is Heads %s is Tales \n", player1, player2);
    printf("Tossing coin.... \n");
    int toss = time(0) % 2;
    if (toss == 0)
    {
        printf("Heads: %s starts\n", player1);
        color = '1';
    }
    else
    {
        printf("Tales: %s starts\n", player2);
        color = '2';
    }
    init_board();
    printBoard();

    while (!checkFull())
    {
        printf("Player %s, your turn!\n", (color == '1') ? player1 : player2);
        clock_t before = clock();
        choose();
        clock_t diff = clock() - before;
        if (color == '1')
            player1Time += diff;
        else
            player2Time += diff;
        printf("\n\n");
        printBoard();
        if (check())
        {
            printf("\n Player  %s wins! \n", (color == '1') ? player1 : player2);
            break;
        }
        Color();
    }
    if (checkFull())
    {
        printf("%s won because his moves was faster.", (player1Time > player2Time) ? player2 : player1);
    }

    return 0;
}

void Color()
{
    color = (color == '1') ? '2' : '1';
}

void init_board()
{
    // fill the board with the empty character:
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLOMNS; j++)
        {
            board[i][j] = '0';
        }
    }
}
// Requires: Nothing
// Effects: prints out the table formated.
void printBoard()
{
    // print the board and the board according to current game:
    printf("%s\n", header);

    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLOMNS; j++)
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
    int colomn = -1;
    while (colomn == -1)
    {
        printf("\nChoose colomn: ");
        scanf(" %c", &c);
        switch (c)
        {
        case '1':
            colomn = 0;
            break;
        case '2':
            colomn = 1;
            break;
        case '3':
            colomn = 2;
            break;
        case '4':
            colomn = 3;
            break;
        case '5':
            colomn = 4;
            break;
        case '6':
            colomn = 5;
            break;
        case '7':
            colomn = 6;
            break;
        default:
            printf("\nWrong colomn number\n");
            printBoard();
        }
        colomn = (fill_bin(colomn) == 1) ? 1 : -1;
    }
}
// Requires: The number of the column the player wants to insert to. The number should be between 1 and 7.
// Effects: Fills out the board where it is required.
int fill_bin(int colomn)
{
    int fail = -1;
    if (colomn == -1)
        return fail;
    int level;

    for (level = ROWS - 1; level >= 0; level--)
    {
        if (board[level][colomn] == '0')
        {
            board[level][colomn] = color;
            fail = 1;
            break;
        }
    }
    if (fail == -1)
        printf("This colomn is full! Please choose another one");
    return fail;
}
// Requires: Nothing.
// Effects: Checks if the winning condition is satisfied vertically.
int checkVertical()
{
    int i, j, k;
    int count = 0;

    for (j = 0; j < COLOMNS; j++)
    {
        for (i = ROWS - 1; i >= 0; i--)
        {
            if (count + i < FOUR)
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
// Effects: Checks if the winning condition is satisfied horizentally.
int checkHorizental()
{
    int i, j, k;
    int count = 0;
    for (i = ROWS - 1; i >= 0; i--)
    {
        for (j = 0; j < COLOMNS; j++)
        {
            if (count + (COLOMNS - j) < FOUR)
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
int checkOblique()
{
    int i, j, k;
    int count = 0;

    int ii, jj;
    for (i = 1; i < ROWS - 1; i++)
    {
        for (j = 1; j < COLOMNS - 1; j++)
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
            for (ii = i + 1, jj = j + 1; (ii <= ROWS - 1) || (jj <= COLOMNS - 1); ii++, jj++)
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
            for (ii = i - 1, jj = j + 1; (ii >= 0) || (jj <= COLOMNS - 1); ii--, j++)
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
    return checkHorizental() || checkVertical() || checkOblique();
}

int checkFull()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLOMNS; j++)
        {
            if (board[i][j] == '0')
                return 0;
        }
    }
    return 1;
}