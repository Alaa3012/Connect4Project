//
// Created by Admin on 11/16/2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FOUR_help 4
#define ROWS_help 6
#define COLUMNS_help 7
#define min_help(a,b) (((a)<(b))?(a):(b))
#define max_help(a,b) (((a)>(b))?(a):(b))

int numPlays_help = 0;
int BotToken_help;
int PlayerToken_help;
/*
 Requires: The bot count, Enemy count, and zero count.
 Effects: returns the score of current board.
 testing: count = 4, countEmpty= 0, countEnemy = 0
          count = 3, countEmpty= 1, countEnemy = 0
          count = 2, countEmpty= 2, countEnemy = 0
          count = 0, countEmpty= 2, countEnemy = 2
          count = 0, countEmpty= 0, countEnemy = 4
          count = 0, countEmpty= 1, countEnemy = 3
 */
int calculateScore_help(int count, int countEmpty, int countEnemy){
    int score = 0;
    if(count == FOUR_help ) score+= 10000000;
    else if(count == 3 && countEmpty == 1) score += 5;
    else if(count == 2 && countEmpty == 2) score += 2;
    if (countEnemy == 2 && countEmpty == 2) score -=1;
    else if (countEnemy == 3 && countEmpty == 1) score -= 4;
    else if (countEnemy == 4) score -=100000;
    return score;
}
/*
 Requires: The current board.
 Effects: Returns the total score of the given board.
 Testing: test a horizontal case
          test a vertical case
          test a positive sloping case
          test a negative sloping case
 */
int scoringMoves_help(int** board){
    int score = 0;
    for (int i = 0; i < ROWS_help; ++i) {
        if(board[i][3] == BotToken_help) score += 3;
    }
    //score horizontal.
    for (int i = 0; i < ROWS_help; ++i) {
        int* row = board[i];
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (row[j+k] == BotToken_help) count++;
                else if (row[j+k] == 0) countEmpty++;
                else if (row[j+k] == PlayerToken_help) countEnemy++;
            }
            score += calculateScore_help(count, countEmpty, countEnemy);
        }
    }
    //score vertical
    for (int i = 0; i < COLUMNS_help; ++i) {
        for (int j = 0; j < 3; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[j+k][i] == BotToken_help) count++;
                else if (board[j+k][i] == 0) countEmpty++;
                else if (board[j+k][i] == PlayerToken_help) countEnemy++;
            }
            score += calculateScore_help(count, countEmpty, countEnemy);

        }
    }
    //positively sloping
    for (int i = 3; i < ROWS_help; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[i-k][j+k] == BotToken_help) count++;
                else if (board[i-k][j+k] == 0) countEmpty++;
                else if (board[i-k][j+k] == PlayerToken_help) countEnemy++;
            }
            score += calculateScore_help(count, countEmpty, countEnemy);
        }
    }
    //negatively sloping
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            int countEmpty = 0;
            int countEnemy = 0;
            for (int k = 0; k <4; k++){
                if (board[i+k][j+k] == BotToken_help) count++;
                else if (board[i+k][j+k] == 0) countEmpty++;
                else if (board[i+k][j+k] == PlayerToken_help) countEnemy++;
            }
            score += calculateScore_help(count, countEmpty, countEnemy);
        }
    }
    return score;
}
/*
 Requires: The current board.
 Effects: Returns a dynamically allocated copy of the board.
 Testing: Give it a board and see if the copy is the same.
 */
int **copyBoard_help(int** board){
    int** copy = (int**)malloc(ROWS_help * sizeof(int*));
    for (int i = 0; i < ROWS_help; ++i) {
        copy[i] = (int*)malloc(COLUMNS_help * sizeof(int));
        for (int j = 0; j < COLUMNS_help; ++j) {
            copy[i][j] = board[i][j];
        }
    }
    return copy;
}
/*
 Requires: A board, required column, and the token to drop.
 Effect: Drop the token at the required column.
 Testing: give it a board and 7 tokens from 1 to 7 and see if it is dropped correctly.
 */
void dropPiece_help(int **board, int column, int token){
    for (int level = ROWS_help - 1; level >= 0; level--){
        if (board[level][column] == 0){ // checks for an empty square from the bottom up in the column.
            board[level][column] = token;
            break;
        }
    }
}

int bestMove_help(int **board, int token){
    int bestScore = -1000;
    int bestCol = clock()%7;
    for (int col = 0; col < COLUMNS_help; ++col) {
        int **copy = copyBoard_help(board);
        if(copy[0][col] == 0){
            dropPiece_help(copy, col, token);
            int score  = scoringMoves_help(copy);
            if(score>bestScore){
                bestScore = score;
                bestCol = col;
            }
        }
        free(copy);
    }
    return bestCol;
}
/*
 Requires: A board and a token.
 Effect: returns 1 if the board contains a winning condition to the corresponding token and 0 otherwise.
 Testing: try horizontal winning
          try vertical winning
          try oblique positive sloping winning
          try oblique negative sloping winning
 */
int won_help(int** board, int token){

    //score horizontal.
    for (int i = 0; i < ROWS_help; ++i) {
        int* row = board[i];
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (row[j+k] == token) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //score vertical
    for (int i = 0; i < COLUMNS_help; ++i) {
        for (int j = 0; j < 3; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[j+k][i] == token) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //positively sloping
    for (int i = 3; i < ROWS_help; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i-k][j+k] == token) count++;
            }
            if(count == FOUR_help) return 1;
        }
    }
    //negatively sloping
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            for (int k = 0; k <4; k++){
                if (board[i+k][j+k] == token) count++;
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
int checkFull_help(int **board){
    for(int j = 0; j < COLUMNS_help; j++){
        if(board[0][j] == 0) return 0; // if one square is equal to 0, then the board is not full
    }
    return 1; // all squares are full.
}
/*
 Requires: A board, the depth, alpha, beta, and maximizing boolean.
 Effects: returns an array of two elements, the first one is the best score and the second is the column corresponding to the best score.
 Testing: Test it on different depths and see how the difficulty changes.
 */
int *Minimax_help(int **board, int depth, int alpha, int beta, int maximizing){
    int botWon = won_help(board, BotToken_help);
    int playerWon = won_help(board, PlayerToken_help);
    int full = checkFull_help(board);

    if( botWon || playerWon || full){
        if(botWon){
            int *result = (int*) malloc(2*sizeof(int));
            result[0] = 1000000000;
            result[1] = -1;
            return result;
        }
        else if(playerWon){
            int *result = (int*) malloc(2*sizeof(int));
            result[0] = -1000000;
            result[1] = -1;
            return result;
        }
        else {
            int *result = (int*) malloc(2*sizeof(int));
            result[0] = 0;
            result[1] = -1;
            return result;
        }
    }
    else if(depth == 0){
        int *result = (int*) malloc(2*sizeof(int));
        result[0] = scoringMoves_help(board);
        result[1] = -1;
        return result;
    }
    if(maximizing){
        int value = INT_MIN;
        int column = clock()%7;
        for (int col = 0; col < COLUMNS_help; ++col) {
            if(!board[0][col]){
                int **copy = copyBoard_help(board);
                dropPiece_help(copy, col, BotToken_help);
                int newScore = Minimax_help(copy, depth - 1, alpha, beta, 0)[0];
                if(newScore > value){
                    value = newScore;
                    column = col;
                }
                alpha = max_help(alpha, value);
                if (alpha >= beta) break;
                free(copy);
            }
        }
        int *result = (int*) malloc(2*sizeof(int));
        result[0] = value;
        result[1] = column;
        return result;
    }
    else{
        int value = INT_MAX;
        int column = clock()%7;
        for (int col = 0; col < COLUMNS_help; ++col) {
            if(!board[0][col]){
                int **copy = copyBoard_help(board);
                dropPiece_help(copy, col, PlayerToken_help);
                int newScore = Minimax_help(copy, depth - 1, alpha, beta, 1)[0];
                if(newScore < value){
                    value = newScore;
                    column = col;
                }
                beta = min_help(beta, value);
                if (alpha >= beta) break;
                free(copy);
            }
        }
        int *result = (int*) malloc(2*sizeof(int));
        result[0] = value;
        result[1] = column;
        return result;
    }
}
/*
 Requires: A board.
 Effects: returns the numbers of plays so far.
 Testing: try an empty board
          try a non-empty board
          try a full board
 */
int numberOfPlays_help(int **board){
    int count = 0;
    for (int i = 0; i < ROWS_help; ++i) {
        for (int j = 0; j < COLUMNS_help; ++j) {
            if (board[i][j] != 0) count ++;
        }
    }
    return count;
}
/*
 Requires: A board and the depth.
 Effects: returns the best move to make determined by that depth.
 Testing: try different depths and see how it affects the gameplay.
 */
int makeMove(int **board, int depth){
    if (!numPlays_help){
        int num = numberOfPlays_help(board);
        if (num){
            BotToken_help = 2;
            PlayerToken_help = 1;
        }
        else{
            BotToken_help = 1;
            PlayerToken_help = 2;
        }
    }
    numPlays_help++;
    return Minimax_help(board, depth + ((numPlays_help - 1) / 2), INT_MIN, INT_MAX, 1)[1];
}
/*
Requires: A board.
Effects: returns the best move to make determined by depth of 8.
Testing: try different depths and see how it affects the gameplay.
 */
int make_move_help(int **board){
    if (!numPlays_help){
        int num = numberOfPlays_help(board);
        if (num){
            BotToken_help = 2;
            PlayerToken_help = 1;
        }
        else{
            BotToken_help = 1;
            PlayerToken_help = 2;
        }
    }
    numPlays_help++;
    return Minimax_help(board, 8 + ((numPlays_help - 1) / 2), INT_MIN, INT_MAX, 1)[1];
}