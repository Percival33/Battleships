#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "ship.h"
#include "board.h"
#include "board_ship_func.h"
#include "player.h"
#include "commands.h"

field_t rotate(field_t field, int* dir, int moveDir, int cls) {
	int newDir = (*dir + moveDir + 4) % 4;

	if (moveDir == F) { // no rotation -> moveDir = F(orward)
		field.x -= dx[*dir];
		field.y -= dy[*dir];
		return field;
	}

	*dir = newDir;

	// N , S -> y +-
	// E , W -> x +-
	switch (newDir) {
		case N:
			field.y -= (cls - 1);
			break;
		case E:
			field.x += (cls - 1);
			break;
		case S:
			field.y += (cls + 1);
			break;
		case W:
			field.x -= (cls - 1);
			break;
	}

	return field;
}

int validate_move(board_t** board, dim_t* dim, ship_t ship, player_t* player, 
	field_t currField, int cls, int currDir, int shipId) {

	if (check_ship_fits_on_board(currField, currDir, cls, player, board, dim) == False) {
		return C_SHIP_WENT_FROM_BOARD;
	}

	if (check_if_free_from_reef(board, dim, currField, cls, currDir) == False) { // after moving forward 1 square
		return C_PLACING_SHIP_ON_REEF;
	}

	if (check_neighbouring_fields(board, currField, dim, cls, currDir) == False) {
		return C_PLACING_SHIP_TOO_CLOSE;
	}

	return True;
}

void add_moved_ship(board_t** board, dim_t* dim, player_t* player, ship_t ship, 
	field_t currField, int currDir, int cls, int shipId) {

	ship.direction = currDir;
	ship.head = currField;
	ship.moved++;

	player->ships[cls][shipId] = ship;

	add_ship_on_board(board, dim, ship.head, player, cls, ship.direction, shipId);
	return;
}

int check_if_ship_can_go(board_t** board, player_t* player, dim_t* dim, int cls, int shipId, int moveDir) { 
	ship_t ship = player->ships[cls][shipId];
	int is_valid;
	int currDir = ship.direction;
	field_t currField = ship.head;

	remove_ship_from_board(board, currField, player, cls, currDir);
	currField = rotate(currField, &currDir, F, cls);
	is_valid = validate_move(board, dim, ship, player, currField, cls, currDir, shipId);

	if (is_valid != True) {
		add_ship_on_board(board, dim, ship.head, player, cls, ship.direction, shipId);
		return is_valid;
	}

	if (moveDir == F) { // move was only forward, adding ship in its new position
		add_moved_ship(board, dim, player, ship, currField, currDir, cls, shipId);
		return True;
	}

	currField = rotate(currField, &currDir, moveDir, cls);
	is_valid = validate_move(board, dim, ship, player, currField, cls, currDir, shipId);

	if (is_valid != True) {
		add_ship_on_board(board, dim, ship.head, player, cls, ship.direction, shipId);
		return is_valid;
	}

	add_moved_ship(board, dim, player, ship, currField, currDir, cls, shipId);

	return True;
}

int get_number_of_moves(int cls) {
	if (cls == S_CAR)
		return CAR_MOVES;
	return OTHER_MOVES;
}

void validate_move_command(char command[], player_t* player, int argc, int cls, int moveDir, int shipId, int extendedShips) {
	if (argc != 3) {
		handle_invalid_command(command, C_INVALID);
	}
	if (cls == ERROR || moveDir == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (shipId >= MAX_SHIPS_NUMBER || player->ships[cls][shipId].placed == False) {
		handle_invalid_command(command, C_INVALID);
	}
	if (player->ships[cls][shipId].moved == get_number_of_moves(cls)) {
		handle_invalid_command(command, C_SHIP_MOVED_ALREADY);
	}
	if (extendedShips == True && player->ships[cls][shipId].damaged[cls - 1] == True) { // engine destroyed
		handle_invalid_command(command, C_SHIP_CANNOT_MOVE);
	}

	return;
}

void move(char command[], board_t** board, dim_t* dim, player_t* player, int extendedShips) {
	//MOVE i C x
	int shipId;
	char clsChar[MAX_SHIP_LENGTH];
	char moveDirChar;

	int argc = sscanf(command, "%*s %d %s %c", &shipId, clsChar, &moveDirChar);

	int cls = get_class(clsChar);
	int moveDir = get_move_dir(moveDirChar);

	assert(0 <= shipId < MAX_SHIPS_NUMBER);

	validate_move_command(command, player, argc, cls, moveDir, shipId, extendedShips);

	int flag = check_if_ship_can_go(board, player, dim, cls, shipId, moveDir);

	if (flag != True) {
		handle_invalid_command(command, flag);
	}

	return;
}
