#pragma once

/*
	TODO: create ship struct 
*/

typedef struct ship{
	//int valid;
	field_t head;
	int direction;
	int damaged[5]; // TODO: should be array?
}ship_t;


int fleetAmt[6]; //fleet[S] number of ships of class S

/*
	handles and validates place_ship command
	TODO: 
		check if ship fits into player's starting position
		check whether ship of this ID is already present
		check if there are already all ships of that type
*/
void place_ship(struct board_t** board, char command[], struct player_t* player);

/*
	handles and validates set_fleet command
*/
void set_fleet(char command[], struct player_t* player);


/*
	function frees memory used in ship stucture
	TODO: fix this function, to free memory used for fleet of player
*/
void fleet_free();

/*
	print number of ships of all types
*/
void print_players_fleet(struct player_t* player);