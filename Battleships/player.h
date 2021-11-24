#pragma once

typedef struct {
	int rowLow;
	int rowHigh;
	int shipPlaced;
	int fleetSize;
}player_t;

int shoot(struct board_t** board, struct dim_t* dim, struct player_t* player, char command[]);
