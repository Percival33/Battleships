#define _CRT_SECURE_NO_WARNINGS
/*
	TODO: create functions to handle:
			- fleet creation 
			- ship placing
			- condition of ships (if it was hit?)
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "ship.h"
#include "board.h"
#include "player.h"

// TODO: check starting position and length with dir
int check_coords(field_t* field, int dir, player_t* player, dim_t* dim) { 
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	int inRows = (player->rowLow <= field->y && field->y <= player->rowHigh);
	int inCols = (0 <= field->x && field->x < COLS);

	if (inRows && inCols) {
		return True;
	}
	return False;
}

int ship_present(int cls, int id) {
	// TODO: check if ship is already present
	return -1;
}

void place_ship(board_t** board, char command[], player_t* player) {
	int y;
	int x;
	char dirChar;
	int dir;
	int id;
	char classChar[10];
	int cls; // class of ship

	int argc = sscanf(command, "%*s %d %d %c %d %s", &y, &x, &dirChar, &id, classChar);
	
	dir = get_dir(dirChar);
	cls = get_class(classChar);

	if (argc != 5) {
		printf("zla comenda place_ship argc: %d\n", argc);
		//TODO: call hanlde invalid command
		return;
	}
	if (cls == -1) {
		//TODO: call hanlde invalid command
	}
	if (check_coords(y, x, dir, player)) {
		//TODO: call hanlde invalid command
	}
	if (ship_present(cls, id)) {
		//TODO: call hanlde invalid command
	}

	//TODO: add ship to board

	//printf("y: %d x: %d dir: %c id: %d category: %d\n", y, x, dir, id, cls);

	return;
}

void print_players_fleet(player_t* player) {
	for (int i = S_DES; i <= S_CAR; i++) {
		printf("Class: %d Amt: %d\n", i, player->fleet[i]);
	}
}

void fleet_free() {
	return;
}