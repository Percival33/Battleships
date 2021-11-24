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

void create_fleet(int fleetSize[], player_t* player) {
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		player->fleet[cls] = fleetSize[cls];

		for (int j = 0; j < 10; j++) {
			player->ships[cls][j] = (ship_t*)malloc(sizeof(ship_t));
			assert(
				player->ships[cls][j] != NULL
			);
			//player->ships[cls][j]->valid = False;
		}

		//for (int j = 0; j < fleetSize[cls]; j++) {
			//player->ships[cls][j]->valid = True;
		//}
	}

	for (int cls = S_DES; cls <= S_CAR; cls++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 5; k++)
				player->ships[cls][j]->damaged[k] = 0;

	player->shipPlaced = 0;
	
	return;
}

void set_fleet(char command[], player_t* player) {
	//int fleetSize[] = { 0,0,4,3,2,1 };
	int fleetSize[6];

	fleetSize[S_DES] = 4;
	fleetSize[S_CRU] = 3;
	fleetSize[S_BAT] = 3;
	fleetSize[S_CAR] = 1;

	int argc = 4;

	/*
		[0] and [1] are additional to make sure that i from fleetSize[i] coresponds to S_TYPE values
			where TYPE is ship type
		[2]: desAmt; default 4
		[3]: cruAmt; default 3
		[4]: batAmt; default 2
		[5]: carAmt; default 1
	*/

	if(command != "") {
		argc = sscanf(command, "%*s %d %d %d %d", &fleetSize[2], &fleetSize[3],
				&fleetSize[4], &fleetSize[5]);
	}

	if (argc != 4) {
		//TODO: wrong number of args
	}
	if (
		(fleetSize[2] < 0 || fleetSize[2] > 10) ||
		(fleetSize[3] < 0 || fleetSize[3] > 10) ||
		(fleetSize[4] < 0 || fleetSize[4] > 10) ||
		(fleetSize[5] < 0 || fleetSize[5] > 10)
		) {
		//TODO: wrong number of ships
	}
	
	//printf("DES: %d CRU: %d BAT: %d CAR: %d\n", fleetSize[2], fleetSize[3], fleetSize[4], fleetSize[5]);

	create_fleet(fleetSize, player);

	//print_fleet(fleet);

	return;
}

void fleet_free() {
	return;
}