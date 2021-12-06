#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "ship.h"
#include "board.h"
#include "player.h"
#include "commands.h"

bool check_coords_inside_player_area(field_t field, int dir, int cls, player_t* player) { 
	
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

bool ship_placed(int cls, int id, player_t* player) {
	for (int i = 0; i < 10; i++) {
		if (player->ships[cls][i].created &&
			player->ships[cls][i].placed &&
			i == id) {
			return True;
		}
	}
	return False;
}

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

	if (check_if_free_from_reef(board, currField, cls, currDir) == False) { // after moving forward 1 square
		return C_PLACING_SHIP_ON_REEF;
	}

	if (check_ship_fits_inside_board(currField, currDir, cls, player, board, dim) == False) {
		return C_SHIP_WENT_FROM_BOARD;
	}
	if (check_neighbouring_fields(board, currField, dim, cls, currDir) == False) {
		return C_PLACING_SHIP_TOO_CLOSE;
	}

	return True;
}

	//TODO refactor function, add_ship in move function
int check_if_free_to_go(board_t** board, player_t* player, dim_t* dim, int cls, int shipId, int moveDir) { 
	ship_t ship = player->ships[cls][shipId];
	int is_valid;
	int currDir = ship.direction;
	field_t currField = ship.head;

	remove_from_board(board, currField, cls, currDir);
	
	currField = rotate(currField, &currDir, F, cls);

	is_valid = validate_move(board, dim, ship, player, currField, cls, currDir, shipId);\

	if (is_valid != True) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return is_valid;
	}

	if (moveDir == F) { // move was only forward, adding ship in its new position
		ship.direction = currDir;
		ship.head = currField;
		ship.moved++;

		player->ships[cls][shipId] = ship;

		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return True;
	}

	currField = rotate(currField, &currDir, moveDir, cls);

	is_valid = validate_move(board, dim, ship, player, currField, cls, currDir, shipId); \

	if (is_valid != True) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return is_valid;
	}

	ship.direction = currDir;
	ship.head = currField;
	ship.moved++;

	player->ships[cls][shipId] = ship;

	add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);

	return True;
}

int get_number_of_moves(int cls) {
	if (cls == S_CAR)
		return CAR_MOVES;
	return OTHER_MOVES;
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

	int flag = check_if_free_to_go(board, player, dim, cls, shipId, moveDir);

	if (flag != True) {
		handle_invalid_command(command, flag);
	}
	
	/*
		1. the ship has not destroyed engine(SHIP CANNOT MOVE),
		2. the ship is not moving too many times(SHIP MOVED ALREADY),
		3. the ship is not placed on reef(PLACING SHIP ON REEF),
		4. the ship not moves out of board (SHIP WENT FROM BOARD),
		5. the ship is not placed too close to other ships (PLACING SHIP TOO CLOSE TO OTHER SHIP).
	*/

}
