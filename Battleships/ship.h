#pragma once
#include "constants.h"

typedef struct ship{
	int created;
	int placed;
	field_t head;
	int direction;
	int damaged[MAX_SHIP_LENGTH]; // TODO: should be array?
}ship_t;


/*
	handles and validates place_ship command
	TODO: 
		check if ship fits into player's starting position
		check whether ship of this ID is already present
		check if there are already all ships of that type
*/
void place_ship(char command[], struct board_t** board, struct player_t* player, struct dim_t* dim);

void set_ship(char command[], struct board_t** board, struct player_t** players, struct dim_t* dim);

