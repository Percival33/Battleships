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

		inRows = (player->rowLow <= field.y && field.y < player->rowHigh);
		inCols = (player->colLow <= field.x && field.x < player->colHigh);

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
	
	player->ships[cls][shipId].placed = True;
	player->ships[cls][shipId].head = field;
	player->ships[cls][shipId].direction = dir;
	player->shipPlaced++;

	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		board[field.y][field.x].playerId = player->id;
		board[field.y][field.x].cls = cls;
		board[field.y][field.x].shipId = shipId;
		board[field.y][field.x].type = B_TAKEN;

		if (len == 0) {
			board[field.y][field.x].type = B_RADAR;
		}
		if (len == 1) {
			board[field.y][field.x].type = B_CANNON;
		}
		if (len + 1 == cls) {
			board[field.y][field.x].type = B_ENGINE;
		}
		if (player->ships[cls][shipId].damaged[len] == True)
			board[field.y][field.x].type = B_DESTROYED;
	}

	return;
}

int check_around(board_t** board, field_t field, dim_t* dim) {
	for (int dir = 0; dir < 4; dir++) {
		int newX = field.x + dx[dir];
		int newY = field.y + dy[dir];

		if (newX >= dim->COLS || newY >= dim->ROWS || newX < 0 || newY < 0) // field does not exist
			continue;

		int fieldType = board[newY][newX].type;

		if (fieldType >= B_TAKEN && fieldType <= B_DESTROYED) { // check if fieldType is occupied by ship
			return False;
		}
	}
	return True;
}

int check_neighbouring_fields(board_t** board, field_t field, dim_t* dim, int cls, int dir) {
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}
		if (!check_around(board, field, dim)) {
			return False;
		}
	}
	return True;
}

int check_if_reef(board_t** board, field_t field, int cls, int dir) {
	for (int len = 0; len < cls; len++) {

		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		if (board[field.y][field.x].type == B_REEF)
			return False;
	}
	return True;
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
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (!check_if_reef(board, field, cls, dir)) {
		handle_invalid_command(command, C_PLACING_SHIP_ON_REEF);
	}
	if (!check_neighbouring_fields(board, field, dim, cls, dir)) { 
		handle_invalid_command(command, C_PLACING_SHIP_TOO_CLOSE);
	}

	assert(player->ships[cls][id].created == True);
	add_ship(board, field, player, cls, dir, id);

	//printf("y: %d x: %d dir: %d id: %d cls: %d\n", y, x, dir, id, cls);

	return;
}

void set_ship(char command[], board_t** board, player_t** players, dim_t* dim) {
	// SHIP P y x D i C a1...al
	char P;
	int playerId;
	int y;
	int x;
	char dirChar;
	int dir;
	int shipId;
	char clsChar[MAX_SHIP_TYPE_NUMBER];
	int cls;
	char statusChar[MAX_SHIP_TYPE_NUMBER];
	int status[MAX_SHIP_TYPE_NUMBER];

	int argc = sscanf(command, "%*s %c %d %d %c %d %s %s", &P, &y, &x, &dirChar, &shipId, clsChar, statusChar);
	playerId = get_player_id(P);
	dir = get_dir(dirChar);
	cls = get_class(clsChar);

	field_t field;
	field.y = y;
	field.x = x;

	if (argc != 7) {
		handle_invalid_command(command, C_INVALID);
	}
	if (playerId != PLAYER_A && playerId != PLAYER_B) {
		handle_invalid_command(command, C_INVALID);
	}
	if (shipId >= players[playerId]->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (!check_if_reef(board, field, cls, dir)) {
		handle_invalid_command(command, C_PLACING_SHIP_ON_REEF);
	}
	if (!check_neighbouring_fields(board, field, dim, cls, dir)) {
		handle_invalid_command(command, C_PLACING_SHIP_TOO_CLOSE);
	}
	if (ship_placed(cls, shipId, players[playerId])) {
		handle_invalid_command(command, C_SHIP_ALREADY_PRESENT);
	}

	ship_t ship = players[playerId]->ships[cls][shipId];

	for (int i = 0; i < cls; i++)
		ship.damaged[i] = (statusChar[i] == '0' ? True : False);

	players[playerId]->ships[cls][shipId] = ship;

	add_ship(board, field, players[playerId], cls, dir, shipId);

	return;
}
