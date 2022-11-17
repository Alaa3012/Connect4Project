//
// Created by Admin on 11/16/2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FOUR 4
#define ROWS 6
#define COLUMNS 7

int BotToken = 2;
int PlayerToken = 1;

int calculateScore(int count, int countEmpty, int countEnemy){
    int score = 0;
    if( count == FOUR ) score+= 100;
    else if(count == 3 && countEmpty == 1) score += 10;
    else if(count == 2 && countEmpty == 2) score += 5;
    if (countEnemy == 3 && countEmpty == 1) score -= 80;
    return score;
}
int scoringMoves(int** board, int token){
    int enemyToken = (token == BotToken)? PlayerToken : BotToken;
    int score = 0;
    for (int i = 0; i < ROWS; ++i) {
        if(board[i][3] == token) score += 6;
    }
    //score horizontal.
    for (int i = 0; i < ROWS; ++i) {
        int* row = board[i];
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (row[j+k] == token) count++;
                else if (row[j+k] == 0) countEmpty++;
                else if (row[j+k] == enemyToken) countEnemy++;
            }
            score += calculateScore(count, countEmpty, countEnemy);
        }
    }
    //score vertical
    for (int i = 0; i < COLUMNS; ++i) {
        for (int j = 0; j < 3; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[j+k][i] == token) count++;
                else if (board[j+k][i] == 0) countEmpty++;
                else if (board[j+k][i] == enemyToken) countEnemy++;
            }
            score += calculateScore(count, countEmpty, countEnemy);

        }
    }
    //positively sloping
    for (int i = 3; i < ROWS; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[i-k][j+k] == token) count++;
                else if (board[i-k][j+k] == 0) countEmpty++;
                else if (board[i-k][j+k] == enemyToken) countEnemy++;
            }
            score += calculateScore(count, countEmpty, countEnemy);
        }
    }
    //negatively sloping
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[i+k][j+k] == token) count++;
                else if (board[i+k][j+k] == 0) countEmpty++;
                else if (board[i+k][j+k] == enemyToken) countEnemy++;
            }
            score += calculateScore(count, countEmpty, countEnemy);
        }
    }
    return score;
}
int **copyBoard(int** board){
    int** copy = (int**)malloc(ROWS * sizeof(int*));
    for (int i = 0; i < COLUMNS; i++)
        copy[i] = (int*)malloc(COLUMNS * sizeof(int));
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            copy[i][j] = board[i][j];
        }
    }
    return copy;
}
void dropPiece(int **board, int column, int token){
    for (int level = ROWS - 1; level >= 0; level--){
        if (board[level][column] == 0){ // checks for an empty square from the bottom up in the column.
            board[level][column] = token;
            break;
        }
    }
}

int bestMove(int **board, int token){
    int bestScore = -1000;
    int bestCol = clock()%7;
    for (int col = 0; col < COLUMNS; ++col) {
        int **copy = copyBoard(board);
        if(copy[0][col] == 0){
            dropPiece(copy,col,token);
            int score  = scoringMoves(copy, token);
            if(score>bestScore){
                bestScore = score;
                bestCol = col;
            }
        }
        free(copy);
    }
    return bestCol;
}

