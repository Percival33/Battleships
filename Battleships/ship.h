#pragma once

typedef struct ship{
	int created;
	int placed;
	field_t head;
	int direction;
	int moved;
	int shots;
	int spies;
	int damaged[MAX_SHIP_LENGTH];
}ship_t;


/*
	handles and validates move command
*/
void move(char command[], struct board_t** board, struct dim_t* dim, struct player_t* player, int extendedShips);

int check_if_ship_can_go(struct board_t** board, struct player_t* player, struct dim_t* dim, int cls, int shipId, int moveDir);