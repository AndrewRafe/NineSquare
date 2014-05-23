/////////////////////////////////////////////////////////////////////////////////////////
/* Author: Andrew Rafe awra057 (z3461633)
 * Version 0.11: A player that plays the current board according to the eval formula:
 * Eval(s) = 3X2(s) + X1(s) - 3(O2(s) + O1(s)) - (10O2(s1) + 3O1(s1))
 * This takes into account the board that will be played next by the opponent
 * where it is very bad if they already have a double and slightly bad if they have a
 * single.
 * DATE: FRI 23 MAY 2014
 */
/////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "common.h"
#include "agent.h"
#include "game.h"

#define MAX_MOVE 81

#define TOP_LEFT 1
#define TOP_MIDDLE 2
#define TOP_RIGHT 3
#define MIDDLE_LEFT 4
#define MIDDLE_MIDDLE 5
#define MIDDLE_RIGHT 6
#define BOTTOM_LEFT 7
#define BOTTOM_MIDDLE 8
#define BOTTOM_RIGHT 9

int board[10][10];
int move[MAX_MOVE+1];
int player;
int m;

//Additional functions not included in agent.h
void clearBoard();
int moveEvaluation(int boardNum, int possibleMove, int currPlayer);
int numOfSingles(int boardNum, int testPlayer, int boardTest[10][10]);
int numOfDoubles(int boardNum, int currPlayer, int boardTest[10][10]);

void usage( char argv0[] )
{
  printf("Usage: %s\n",argv0);
  printf("       [-p port]\n"); // tcp port
  printf("       [-h host]\n"); // tcp host
  exit(1);
}

//  parse command-line arguments
void agent_parse_args( int argc, char *argv[] ) {
	int i=1;
  	while( i < argc ) {
    	if( strcmp( argv[i], "-p" ) == 0 ) {
			if( i+1 >= argc ) {
        		usage( argv[0] );
    		}
    		port = atoi(argv[i+1]);
    		i += 2;
    	} else if( strcmp( argv[i], "-h" ) == 0 ) {
    		if( i+1 >= argc ) {
        		usage( argv[0] );
      		}
			host = argv[i+1];
      		i += 2;
    	} else {
      		usage( argv[0] );
    	}
  	}
}

//  called at the beginning of a series of games
void agent_init() {
	//DO NOTHING YET
}

//  called at the beginning of each game
void agent_start( int this_player ) {
	//Wipe the board
	clearBoard();
	m = 0;
	move[m] = 0;
	player = this_player;
}

int  agent_second_move(int board_num, int prev_move ) {
	//Store the initial move made
	move[m] = board_num;
	m++;
	move[m] = prev_move;
	board[move[m-1]][prev_move] = OPPONENT;
	m++;
	int moveToMake = 0;
	int i;
	
	//Will pick the first available move
	for (i = 1; i < 10; i++) {
		if (board[move[m-1]][i] == EMPTY) {
			moveToMake = i;
			break;
		}
	}
	
	//Add the move to the board under the player move
	move[m] = moveToMake;
	board[move[m-1]][moveToMake] = PLAYER;
	m++;
	
	printf("Making move: %d\n", moveToMake);
	return moveToMake;
	
}

int  agent_third_move(int board_num,int first_move,int prev_move) {
	
	move[m] = board_num;
	m++;
	move[m] = first_move;
	board[move[m-1]][first_move] = PLAYER;
	m++;
	move[m] = prev_move;
	board[move[m-1]][prev_move] = OPPONENT;
	m++;
	
	int moveToMake = 0;
	int i;

	//Will pick the first available move
	for (i = 1; i < 10; i++) {
		if (board[move[m-1]][i] == EMPTY) {
			moveToMake = i;
			break;
		}
	}
	
	//Add the move to the board under the player move
	move[m] = moveToMake;
	board[move[m-1]][moveToMake] = PLAYER;
	m++;
	
	printf("Making move: %d\n", moveToMake);
	return moveToMake;

}

int  agent_next_move( int prev_move ) {
	
	//Add the move information to knowledge
	move[m] = prev_move;
	board[move[m-1]][prev_move] = OPPONENT;
	m++;

	int i, j;
	int possibleMoves[10];
	
	//Set all of the possibleMoves to -1
	for (j = 0; j < 10; j++) {
		possibleMoves[j] = -1;
	}
	
	j = 0;
	//Will pick the first available move
	for (i = 1; i < 10; i++) {
		if (board[move[m-1]][i] == EMPTY) {
			possibleMoves[j] = i;
			j++;
		}
	}
	
	int bestMove, bestEval;
	int testMove, testEval;
	
	bestMove = possibleMoves[0];
	bestEval = moveEvaluation(move[m-1], bestMove, PLAYER);
	testMove = possibleMoves[1];
	j = 2;
	
	while (testMove != -1) {
		testEval = moveEvaluation(move[m-1], testMove, PLAYER);
		if (testEval > bestEval) {
			bestEval = testEval;
			bestMove = testMove;
		}
		testMove = possibleMoves[j];
		j++;
	}
	
	//Add the move to the board under the player move
	move[m] = bestMove;
	board[move[m-1]][bestMove] = PLAYER;
	m++;
	
	printf("Making move: %d\n", bestMove);
	return bestMove;
}

void agent_last_move( int prev_move ) {
	//Add the final move to the board and finish
	move[m] = prev_move;
  	board[move[m-1]][move[m]] = OPPONENT;
  	m++;
}

 //  called at the end of each game
void agent_gameover( int result, int cause ) {
	//DO NOTHING YET
}

 //  called at the end of the series of games
void agent_cleanup() {
	//DO NOTHING YET
}

// A method to clean the board and fill it with EMPTY
void clearBoard() {
	int i, j;
	for (i = 1; i < 10; i++) {
		for (j = 1; j < 10; j++) {
			board[i][j] = EMPTY;
		}
	}
} 

int moveEvaluation(int boardNum, int possibleMove, int currPlayer) {
	int moveScore;
	int boardTest[10][10];
	int i, j;
	for (i = 1; i < 10; i++) {
		for (j = 1; j < 10; j++) {
			boardTest[i][j] = board[i][j];
		}
	}
	boardTest[boardNum][possibleMove] = currPlayer;
	
	moveScore = ( 3*numOfDoubles(boardNum, PLAYER, boardTest) + numOfSingles(boardNum, PLAYER, boardTest) -
					3*(numOfDoubles(boardNum, OPPONENT, boardTest) + numOfSingles(boardNum, OPPONENT, boardTest)) -
					(10*numOfDoubles(possibleMove, OPPONENT, boardTest) + 2*numOfSingles(possibleMove, OPPONENT, boardTest)));
	
	printf("Move %d evaluated to %d\n", possibleMove, moveScore);
	return moveScore;
}


int numOfSingles(int boardNum, int testPlayer, int boardTest[10][10]) {
	
	int singles = 0;

	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][TOP_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][TOP_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][TOP_RIGHT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY) ||
			(boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY) ||
			(boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][BOTTOM_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][BOTTOM_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][BOTTOM_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == EMPTY) ||
			(boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] == EMPTY) ||
			(boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		singles++;
	}
	if ((boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == testPlayer)) {
		singles++;
	}
	return singles;
}



// A method to return the number of doubles on a particular one of the nine boards for
// a given fake board
int numOfDoubles(int boardNum, int testPlayer, int boardTest[10][10]) {
	
	int doubles = 0;
	//If two of the board positions in the rows, columns or diagonals belong to the current testPlayer and the third position does not belong to the opponent
	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][TOP_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][TOP_RIGHT] == testPlayer) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][TOP_RIGHT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY) ||
			(boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer) ||
			(boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][BOTTOM_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][BOTTOM_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer) ||
			(boardTest[boardNum][BOTTOM_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_LEFT] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == testPlayer) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_LEFT] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] ==EMPTY) ||
			(boardTest[boardNum][TOP_MIDDLE] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer) ||
			(boardTest[boardNum][TOP_MIDDLE] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_MIDDLE] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_RIGHT] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_RIGHT] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == EMPTY) ||
			(boardTest[boardNum][TOP_LEFT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer) ||
			(boardTest[boardNum][TOP_LEFT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_RIGHT] == testPlayer)) {
		doubles++;
	}
	if ((boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == EMPTY) ||
			(boardTest[boardNum][TOP_RIGHT] == testPlayer && boardTest[boardNum][MIDDLE_MIDDLE] == EMPTY && boardTest[boardNum][BOTTOM_LEFT] == testPlayer) ||
			(boardTest[boardNum][TOP_RIGHT] == EMPTY && boardTest[boardNum][MIDDLE_MIDDLE] == testPlayer && boardTest[boardNum][BOTTOM_LEFT] == testPlayer)) {
		doubles++;
	}
	
	return doubles;
}
