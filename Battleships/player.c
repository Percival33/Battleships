#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "player.h"
#include "board.h"

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
	if (player->shipPlaced != player->fleetSize) {
		printf("NOT ALL SHIPS PLACED\n");
		//TODO: NOT ALL SHIP PLACED
	}

	printf("y: %d x: %d\n", field.y, field.x);

	return 1;
}