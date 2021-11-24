#pragma once
#include "ship.h"

typedef struct player{
	int rowLow; 
	int rowHigh;
	int shipPlaced;			// number of ship placed
	int fleet[6];		// fleet[S] = number of ships of class S
	ship_t* ships[6][10];		// ships[S][ID] = ships of class S with ID
}player_t;	

player_t* player_init(int rowLow, int rowHigh);

int shoot(struct board_t** board, struct dim_t* dim, struct player_t* player, char command[]);

/*
	Function counts all remaining parts of ships
*/
int get_remaining_parts(player_t* player);

void go_default_fleet(player_t* player[]);


/*
	Function sets nextPlayer or calls handle invalid command
*/
int set_next_player(char command[]);