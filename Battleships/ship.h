#pragma once

typedef struct ship{
	int created;
	int placed;
	field_t head;
	int direction;
	int moved;
	int damaged[MAX_SHIP_LENGTH];
}ship_t;


/*
	handles and validates place_ship command
*/
void place_ship(char command[], struct board_t** board, struct player_t* player, struct dim_t* dim);

void set_ship(char command[], struct board_t** board, struct player_t** players, struct dim_t* dim);

void move(char command[], struct board_t** board, struct dim_t* dim, struct player_t* player);