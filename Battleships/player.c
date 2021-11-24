#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "commands.h"
#include "ship.h"
#include "player.h"
#include "board.h"

player_t* player_init(int rowLow, int rowHigh) {
	player_t* player = (player_t*)malloc(sizeof(player_t));

	assert(player != NULL);

	player->rowLow = rowLow;
	player->rowHigh = rowHigh;

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
			for (int k = 0; k < cls; k++)
				size += 1 - player->ships[cls][j]->damaged[k];
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

void go_default_fleet(player_t* player[]) {
	set_fleet("", player[PLAYER_A]);
	set_fleet("", player[PLAYER_B]);
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
