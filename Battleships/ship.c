/*
	TODO: create functions to handle:
			- fleet creation 
			- ship placing
			- condition of ships (if it was hit?)
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "ship.h"
#include "board.h"
#include "player.h"

int check_coords(int y, int x, int dir, player_t* player) {
	// TODO: check starting position
	return -1;
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
	else if (cls == -1) {
		//TODO: call hanlde invalid command
	}
	else if (check_coords(y, x, dir, player)) {
		//TODO: call hanlde invalid command
	}
	else if (ship_present(cls, id)) {
		//TODO: call hanlde invalid command
	}


	printf("y: %d x: %d dir: %c id: %d category: %d\n", y, x, dir, id, cls);

	return;
}