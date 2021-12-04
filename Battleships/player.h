#pragma once
#include "ship.h"
#include "constants.h"

typedef struct player {
	int id;
	int rowLow; 
	int rowHigh;
	int colLow;
	int colHigh;
	int shipPlaced;												// number of ship placed
	int fleet[MAX_SHIP_TYPE_NUMBER];							// fleet[S] = number of ships of class S
	ship_t ships[MAX_SHIP_TYPE_NUMBER][MAX_SHIPS_NUMBER];		// ships[S][ID] = ships of class S with ID
} player_t;	

player_t* player_init(int rowLow, int rowHigh, int id);

/*
	Free memory allocated for player structures
*/
void player_free(player_t** players);


/*
	Function handles input validates and shoot field, if EXTENDED_SHIPS if off
*/
void shoot_default(char command[], struct board_t** board, struct dim_t* dim, struct player_t** players, int playerId);

/*
	Function handles input validates and shoot field, if EXTENDED_SHIPS if on
*/
void shoot_extended(char command[], struct board_t** board, struct dim_t* dim, struct player_t** players, int playerId);

/*
	Function counts all remaining parts of ships
*/
int get_remaining_parts(player_t* player);

/*
	Function sets nextPlayer or calls handle invalid command
*/
int set_next_player(char command[]);


/*
	Function sets fleet size for specific player.
	It also validates command
*/
void set_fleet(char command[], player_t** player);


/*
	Function returns number of ships of player.
*/
int get_fleet_size(player_t* player);