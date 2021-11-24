#pragma once

/*
	TODO: create ship struct 
*/

typedef struct {
	int valid;
	field_t head;
	int direction;
	int* damaged; // TODO: should be array?
}ship_t;


ship_t fleet[6][10]; //fleet[S][ID] ship of class S with id = ID 

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
	TODO:
		by default set CAR:1 BAT:2 CRU:3 DES:4

*/
void set_fleet(char command[], struct player_t* player);


/*
	function frees memory used in ship stucture
*/
void fleet_free(ship_t fleet[6][10]);