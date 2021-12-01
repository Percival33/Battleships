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
#include "commands.h"

int check_coords_inside(field_t field, int dir, int cls, player_t* player, board_t** board,
						dim_t* dim) { 
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	int inRows;
	int inCols;

	for (int len = 0; len < cls; len++) {
		
		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		inRows = (player->rowLow <= field.y && field.y <= player->rowHigh);
		inCols = (0 <= field.x && field.x < COLS);

		if (!inRows || !inCols)
			return False;
	
	}
	return True;
}

int ship_placed(int cls, int id, player_t* player) {
	for (int i = 0; i < 10; i++) {
		if (player->ships[cls][i].created &&
			player->ships[cls][i].placed &&
			i == id) {
			return True;
		}
	}
	return False;
}

void add_ship(board_t** board, field_t field, player_t* player, int cls, int dir, int shipId) { 

	player->ships[cls][shipId].head = field;
	player->ships[cls][shipId].placed = True;
	player->ships[cls][shipId].direction = dir;
	player->shipPlaced++;

	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		board[field.y][field.x].type = B_TAKEN;
		board[field.y][field.x].playerId = player->id;
		board[field.y][field.x].cls = cls;
		board[field.y][field.x].shipId = shipId;

	}

	return;
}

void place_ship(char command[], board_t** board, player_t* player, dim_t* dim) {
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

	field_t field;
	field.y = y;
	field.x = x;

	if (argc != 5 || cls == -1) {
		printf("zla comenda place_ship argc: %d\n", argc);
		handle_invalid_command(command, C_INVALID);
		return;
	}
	if (!check_coords_inside(field, dir, cls, player, board, dim)) {
		handle_invalid_command(command, C_NOT_IN_STARTING_POSITION);
	}
	if (ship_placed(cls, id, player)) {
		handle_invalid_command(command, C_SHIP_ALREADY_PRESENT);
	}
	if (id >= player->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_PRESENT);
	}

	assert(player->ships[cls][id].created == True);
	add_ship(board, field, player, cls, dir, id);
	

	//printf("y: %d x: %d dir: %d id: %d cls: %d\n", y, x, dir, id, cls);

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