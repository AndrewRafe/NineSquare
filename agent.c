/////////////////////////////////////////////////////////////////////////////////////////
/* Author: Andrew Rafe awra057 (z3461633)
 * Version 0.01: Will be the initial bare bones player who will pick the first
 * available move
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

int board[10][10];
int move[MAX_MOVE+1];
int player;
int m;

//Additional functions not included in agent.h
void clearBoard();

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
