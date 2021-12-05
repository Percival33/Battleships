#define _CRT_SECURE_NO_WARNINGS

/*
	TODO: move functions to adequate files
*/

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

bool check_on_board(dim_t* dim, field_t field) {
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	int inRows = (0 <= field.y && field.y < ROWS);
	int inCols = (0 <= field.x && field.x < COLS);
	
	if (inRows && inCols)
		return True;
	return False;
}

bool check_coords_inside_board(field_t field, int dir, int cls, player_t* player, board_t** board,
	dim_t* dim) {
	
	for (int len = 0; len < cls; len++) {

		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}
		
		if (check_on_board(dim, field) == False)
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

//TODO remove visibility if radar is broken
void set_visible(board_t** board, dim_t* dim, field_t target, field_t origin, int range, int playerId) { 
	if (check_on_board(dim, target) == False)
		return;
	if (board[target.y][target.x].visited == True)
		return;
	if (get_dist(target, origin) > range)
		return;

	board[target.y][target.x].visited = True;

	int visible = board[target.y][target.x].visible;
	
	// if ship continue ?

	if (visible == B_EMPTY) { // default value -> seen for the first time
		visible = playerId;
	}
	else if(visible != playerId) {
		assert(visible != B_EMPTY);
		assert(visible == playerId ^ 1);
		visible = B_VISIBLE_BOTH;
	}
	
	assert(visible != B_EMPTY);
	board[target.y][target.x].visible = visible;

	for (int dir = N; dir <= NW; dir++) {
		field_t newTarget;
		newTarget.x = target.x + dx[dir];
		newTarget.y = target.y + dy[dir];

		set_visible(board, dim, newTarget, origin, range, playerId);
	}
	assert(board[target.y][target.x].visible != B_EMPTY);
	return;
}

void clear_visited(board_t** board, dim_t* dim) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;


	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = COL_LOW; col < COL_HIGH; col++) {
			board[row][col].visited = B_EMPTY;
		}
	}

	return;
}

void add_visible_fields(board_t** board, dim_t* dim, player_t* player, int cls, int shipId) {
	field_t field = player->ships[cls][shipId].head;
	int dir = player->ships[cls][shipId].direction;

	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.x += dx[dir];
			field.y += dy[dir];
		}
		
		set_visible(board, dim, field, field, cls*cls, player->id); // origin = field, range = class squared
	}

	clear_visited(board, dim);

	return;
}

void add_ship(board_t** board, dim_t* dim, field_t field, player_t* player, int cls, int dir, int shipId) { 
	
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

	add_visible_fields(board, dim, player, cls, shipId);

	return;
}

bool check_around(board_t** board, field_t field, dim_t* dim) {
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

bool check_neighbouring_fields(board_t** board, field_t field, dim_t* dim, int cls, int dir) {
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

bool check_if_free_from_reef(board_t** board, field_t field, int cls, int dir) {
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

	if (argc != 5) {
		handle_invalid_command(command, C_INVALID);
	}
	if(cls == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (!check_coords_inside_player_area(field, dir, cls, player)) {
		handle_invalid_command(command, C_NOT_IN_STARTING_POSITION);
	}
	if (ship_placed(cls, id, player)) {
		handle_invalid_command(command, C_SHIP_ALREADY_PRESENT);
	}
	if (id >= player->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (check_if_free_from_reef(board, field, cls, dir) == False) {
		handle_invalid_command(command, C_PLACING_SHIP_ON_REEF);
	}
	if (!check_neighbouring_fields(board, field, dim, cls, dir)) { 
		handle_invalid_command(command, C_PLACING_SHIP_TOO_CLOSE);
	}

	assert(player->ships[cls][id].created == True);
	add_ship(board, dim, field, player, cls, dir, id);

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
	if(cls == ERROR || playerId == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (shipId >= players[playerId]->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (check_if_free_from_reef(board, field, cls, dir) == False) {
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

	add_ship(board, dim, field, players[playerId], cls, dir, shipId);

	return;
}

void remove_from_board(board_t** board, field_t field, int cls, int dir) {
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.x += dx[dir];
			field.y += dy[dir];
		}
		board[field.y][field.x].type = B_EMPTY;
		board[field.y][field.x].cls = S_NULL;
		board[field.y][field.x].playerId = B_EMPTY;
		board[field.y][field.x].shipId = S_NULL;
	}
	return;
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

int check_if_free_to_go(board_t** board, player_t* player, dim_t* dim, int cls, int shipId, int moveDir) { //TODO refactor function, add_ship in move function
	ship_t ship = player->ships[cls][shipId];

	int currDir = ship.direction;
	field_t currField = ship.head;

	remove_from_board(board, currField, cls, currDir);
	
	currField = rotate(currField, &currDir, F, cls);

	if (check_if_free_from_reef(board, currField, cls, currDir) == False) { // after moving forward 1 square
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_PLACING_SHIP_ON_REEF;
	}

	if (check_coords_inside_board(currField, currDir, cls, player, board, dim) == False) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_SHIP_WENT_FROM_BOARD;
	}
	if (check_neighbouring_fields(board, currField, dim, cls, currDir) == False) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_PLACING_SHIP_TOO_CLOSE;
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

	if (check_if_free_from_reef(board, currField, cls, currDir) == False) { // after moving forward and rotating
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_PLACING_SHIP_ON_REEF;
	}

	if (check_coords_inside_board(currField, currDir, cls, player, board, dim) == False) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_SHIP_WENT_FROM_BOARD;
	}
	if (check_neighbouring_fields(board, currField, dim, cls, currDir) == False) {
		add_ship(board, dim, ship.head, player, cls, ship.direction, shipId);
		return C_PLACING_SHIP_TOO_CLOSE;
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
