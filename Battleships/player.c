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

	player->id = id;
	player->rowLow = rowLow;
	player->rowHigh = rowHigh;
	player->shipPlaced = 0;

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < MAX_SHIPS_NUMBER; j++) {
			player->ships[cls][j].created = False;
			player->ships[cls][j].placed = False;
			for (int len = 0; len < MAX_SHIP_LENGTH; len++) {
				player->ships[cls][j].damaged[len] = False;
			}
		}
	}

	return player;
}

void player_free(player_t** players) {
	free(players[PLAYER_A]);
	free(players[PLAYER_B]);
	free(players);
	return;
}

int get_fleet_size(player_t* player) {
	int size = 0;
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		size += player->fleet[cls];
	}
	return size;
}

int get_remaining_parts(player_t* player) {
	int size = 0;
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {

			ship_t currShip = player->ships[cls][shipId];

			if (currShip.created == True && currShip.placed == True) {
				for (int len = 0; len < cls; len++) {
					if (currShip.damaged[len] == False) {
						size++;
					}
				}
			}

		}
	}
	return size;
}

int inside(dim_t* dim, field_t* field) {
	int inRows = (0 <= field->y && field->y < dim->ROWS);
	int inCols = (0 <= field->x && field->x < dim->COLS);

	return inRows && inCols;
}

void damage_ship(board_t** board, player_t** players, field_t field) {
	if (board[field.y][field.x].type == B_EMPTY) {
		return;
	}

	board[field.y][field.x].type = B_DESTROYED;
	int playerId = board[field.y][field.x].playerId;
	int cls = board[field.y][field.x].cls;
	int shipId = board[field.y][field.x].shipId;

	ship_t currShip = players[playerId]->ships[cls][shipId];
	field_t head = currShip.head;
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			head.y += dy[currShip.direction];
			head.x += dx[currShip.direction];
		}

		if (head.y == field.y && head.x == field.x) {
			currShip.damaged[len] = True;
		}
	}

	players[playerId]->ships[cls][shipId] = currShip;

	return;
}

int shoot(char command[], board_t** board, dim_t* dim, player_t** players, int playerId) {
	field_t field;
	int argc = sscanf(command, "%*s %d %d", &field.y, &field.x);

	assert(players[playerId]->shipPlaced <= get_fleet_size(players[playerId]));

	if (argc != 2) {
		handle_invalid_command(command, C_INVALID);
	}
	if (!inside(dim, &field)) {
		//printf("FIELD DOES NOT EXIST\n");
		handle_invalid_command(command, C_FIELD_DOES_NOT_EXIST);
	}
	if (players[PLAYER_A]->shipPlaced + players[PLAYER_B]->shipPlaced != 
		get_fleet_size(players[PLAYER_A]) + get_fleet_size(players[PLAYER_B])) {
		handle_invalid_command(command, C_NOT_ALL_SHIPS_PLACED);
	}

	//printf("y: %d x: %d\n", field.y, field.x);

	damage_ship(board, players, field);

	return True;
}

void create_fleet(int fleetSize[], player_t* player) {
	assert(player != NULL);
	assert(sizeof(player->fleet[2]) == sizeof(int));

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		player->fleet[cls] = fleetSize[cls];
	}

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < MAX_SHIPS_NUMBER; j++) {
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

	if (argc != 5								|| // wrong number of argc
		(id == 999)								|| // wrong ID
		(fleetSize[2] < 0 || fleetSize[2] > 10) || // wrong number of given ship class
		(fleetSize[3] < 0 || fleetSize[3] > 10) ||
		(fleetSize[4] < 0 || fleetSize[4] > 10) ||
		(fleetSize[5] < 0 || fleetSize[5] > 10)
		) {											

		handle_invalid_command(command, C_INVALID);
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
