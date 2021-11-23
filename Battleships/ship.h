#pragma once
#include "constants.h"
#include "board.h"
#include "player.h"

/*
	TODO: create ship struct 
*/

typedef struct {
	int type;
	point_t head;
	int direction;
	int damaged; // should be array
}ship_t;


/*
	handles and validates place_ship command
	TODO: 
		check if ship fits into player's starting position
		check whether ship of this ID is already present
		check if there are already all ships of that type
*/
void place_ship(board_t** board, char command[], player_t* player);