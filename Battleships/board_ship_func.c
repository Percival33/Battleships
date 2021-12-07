#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "player.h"
#include "board.h"
#include "board_ship_func.h"

void remove_ship_from_board(board_t** board, field_t field, player_t* player, int cls, int dir) {
	player->shipPlaced--;

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

void add_ship_on_board(board_t** board, dim_t* dim, field_t field, player_t* player, int cls, int dir, int shipId) {

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

bool validate_place_ship_command(char command[], board_t** board, dim_t* dim, field_t field, 
	player_t* player, int argc, int cls, int dir, int shipId)
{
	if (argc != 5) {
		handle_invalid_command(command, C_INVALID);
	}
	if (cls == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (!check_coords_inside_player_area(field, dir, cls, player)) {
		handle_invalid_command(command, C_NOT_IN_STARTING_POSITION);
	}
	if (is_ship_placed(cls, shipId, player)) {
		handle_invalid_command(command, C_SHIP_ALREADY_PRESENT);
	}
	if (shipId >= player->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (check_if_free_from_reef(board, dim, field, cls, dir) == False) {
		handle_invalid_command(command, C_PLACING_SHIP_ON_REEF);
	}
	if (!check_neighbouring_fields(board, field, dim, cls, dir)) {
		handle_invalid_command(command, C_PLACING_SHIP_TOO_CLOSE);
	}

	return True;
}

void place_ship(char command[], board_t** board, player_t* player, dim_t* dim) {
	int y;
	int x;
	char dirChar;
	int dir;
	int shipId;
	char classChar[10];
	int cls; // class of ship

	int argc = sscanf(command, "%*s %d %d %c %d %s", &y, &x, &dirChar, &shipId, classChar);

	dir = get_dir(dirChar);
	cls = get_class(classChar);

	field_t field;
	field.y = y;
	field.x = x;

	validate_place_ship_command(command, board, dim, field, player, argc, cls, dir, shipId);

	assert(player->ships[cls][shipId].created == True);
	add_ship_on_board(board, dim, field, player, cls, dir, shipId);

	return;
}

bool validate_set_ship(char command[], board_t** board, dim_t* dim, field_t field,
	player_t** players, int playerId, int argc, int cls, int dir, int shipId) {
	if (argc != 7) {
		handle_invalid_command(command, C_INVALID);
	}
	if (cls == ERROR || playerId == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (shipId >= players[playerId]->fleet[cls]) {
		handle_invalid_command(command, C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET);
	}
	if (check_if_free_from_reef(board, dim, field, cls, dir) == False) {
		handle_invalid_command(command, C_PLACING_SHIP_ON_REEF);
	}
	if (!check_neighbouring_fields(board, field, dim, cls, dir)) {
		handle_invalid_command(command, C_PLACING_SHIP_TOO_CLOSE);
	}
	if (is_ship_placed(cls, shipId, players[playerId])) {
		handle_invalid_command(command, C_SHIP_ALREADY_PRESENT);
	}

	return True;
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

	validate_set_ship(command, board, dim, field, players, playerId, argc, cls, dir, shipId);

	ship_t ship = players[playerId]->ships[cls][shipId];

	for (int i = 0; i < cls; i++)
		ship.damaged[i] = (statusChar[i] == '0' ? True : False);

	players[playerId]->ships[cls][shipId] = ship;

	add_ship_on_board(board, dim, field, players[playerId], cls, dir, shipId);

	return;
}