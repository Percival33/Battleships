#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "commands.h"
#include "ship.h"
#include "player.h"
#include "board.h"

player_t* player_init(int rowLow, int rowHigh, int id) {
	player_t* player = (player_t*)malloc(sizeof(player_t));

	assert(player != NULL);

	player->rowLow = rowLow;
	player->rowHigh = rowHigh;
	player->shipPlaced = 0;

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < SHIP_MAX_NUMBER; j++) {
			player->ships[cls][j].created = False;
		}
	}

	return player;
}

int get_fleet_size(player_t* player) {
	int size = 0;
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		size += player->fleet[cls] * cls;
	}
	return size;
}

int get_remaining_parts(player_t* player) {
	int size = 0;
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < player->fleet[cls]; j++) {
			if (player->ships[cls][j].created == True) {
				for (int k = 0; k < cls; k++)
					size += 1 - player->ships[cls][j].damaged[k];
			}
		}
	}
	return size;
}

int shoot(board_t** board, dim_t* dim, player_t* player, char command[]) {
	field_t field;
	int argc;

	argc = sscanf(command, "%*s %d %d", &field.y, &field.x);

	if (argc != 2) {
		//TODO: wrong num of args
	}
	if (!board_inside(dim, &field)) {
		printf("FIELD DOES NOT EXIST\n");
		//TODO: not inside of board
		//		FIELD DOES NOT EXIST
	}
	if (player->shipPlaced != get_fleet_size(player)) {
		printf("NOT ALL SHIPS PLACED\n");
		//TODO: NOT ALL SHIP PLACED
	}

	printf("y: %d x: %d\n", field.y, field.x);

	return 1;
}

void create_fleet(int fleetSize[], player_t* player) {
	assert(player != NULL);
	assert(sizeof(player->fleet[2]) == sizeof(int));

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		player->fleet[cls] = fleetSize[cls];
	}

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < SHIP_MAX_NUMBER; j++) {
			player->ships[cls][j].created = False;
		}
	}

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < fleetSize[cls]; j++) {
			player->ships[cls][j].created = True;
			for (int k = 0; k < cls; k++) {
				player->ships[cls][j].damaged[k] = 0;
			}
		}
	}

	player->shipPlaced = 0;

	return;
}

void set_fleet(char command[], player_t** players) {
	int fleetSize[6];
	char P;

	// default values
	//fleetSize[S_DES] = 4;
	//fleetSize[S_CRU] = 3;
	//fleetSize[S_BAT] = 2;
	//fleetSize[S_CAR] = 1;

	int argc = 5;
	int id = -1;


	argc = sscanf(command, "%*s %c %d %d %d %d", &P, &fleetSize[5], &fleetSize[4], &fleetSize[3], &fleetSize[2]);

	if (P == 'A') {
		id = PLAYER_A;
	}
	else if (P == 'B') {
		id = PLAYER_B;
	}
	else {
		id = 999;
	}
	
	assert(id != -1);

	if (argc != 5) {
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

	//printf("P: %d DES: %d CRU: %d BAT: %d CAR: %d\n", id, fleetSize[2], fleetSize[3], fleetSize[4], fleetSize[5]);


	create_fleet(fleetSize, players[id]);

	return;
}

int set_next_player(char command[]) {
	char P;
	int argc = sscanf(command, "%*s %c", &P);
	
	assert(argc == 1);
	
	if (P == 'A') {
		return C_PLAYER_A;
	}
	else if (P == 'B') {
		return C_PLAYER_B;
	}
	else {
		handle_invalid_command(command, C_INVALID);
	}

	return C_INVALID;
}
