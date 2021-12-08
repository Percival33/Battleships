#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "commands.h"
#include "ship.h"
#include "player.h"
#include "board.h"

void set_default_fleet(player_t** players) {
	set_fleet("SET_FLEET A 1 2 3 4", players);
	set_fleet("SET_FLEET B 1 2 3 4", players);
	return;
}

player_t** init_players(dim_t* dim) {
	player_t** players = (player_t**)malloc(2 * sizeof(player_t*));
	assert(players != NULL);

	players[PLAYER_A] = player_init(0, (dim->ROWS / 2), PLAYER_A);
	players[PLAYER_B] = player_init((dim->ROWS / 2) + 1, dim->ROWS, PLAYER_B);

	return players;
}

void check_winner(player_t** players) {
	if (players[PLAYER_A]->shipPlaced + players[PLAYER_B]->shipPlaced !=
		get_fleet_size(players[PLAYER_A]) + get_fleet_size(players[PLAYER_B]))
		return;
	int AReamaining = get_remaining_parts(players[PLAYER_A]);
	int BReamaining = get_remaining_parts(players[PLAYER_B]);

	if (AReamaining == 0) {
		printf("B won\n");
		exit(0);
	}
	if (BReamaining == 0) {
		printf("A won\n");
		exit(0);
	}
	return;
}

void clear_moved(player_t* player) {
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
			player->ships[cls][shipId].moved = 0;
		}
	}
	return;
}

void clear_shots(player_t* player) {
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
			player->ships[cls][shipId].shots = 0;
		}
	}
	return;
}

void clear_spies(player_t* player) {
	for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
		player->ships[S_CAR][shipId].spies = 0;
	}
	return;
}

player_t* player_init(int rowLow, int rowHigh, int id) {
	player_t* player = (player_t*)malloc(sizeof(player_t));

	assert(player != NULL);

	player->isAi = ERROR;
	player->id = id;
	player->rowLow = rowLow;
	player->rowHigh = rowHigh;
	player->colLow = 0;
	player->colHigh = DEFAULT_COLS_NUMBER;

	player->shipPlaced = 0;

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int j = 0; j < MAX_SHIPS_NUMBER; j++) {
			player->ships[cls][j].created = False;
			player->ships[cls][j].placed = False;
			player->ships[cls][j].moved = 0;
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

void damage_ship(board_t** board, player_t** players, field_t target) {
	if (board[target.y][target.x].type == B_EMPTY) {
		return;
	}

	board[target.y][target.x].type = B_DESTROYED;
	int playerId = board[target.y][target.x].playerId;
	int cls = board[target.y][target.x].cls;
	int shipId = board[target.y][target.x].shipId;

	assert(cls != S_NULL);

	ship_t currShip = players[playerId]->ships[cls][shipId];
	field_t field = currShip.head;
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.y += dy[currShip.direction];
			field.x += dx[currShip.direction];
		}

		if (field.y == target.y && field.x == target.x) {
			currShip.damaged[len] = True;
			break;
		}
	}

	players[playerId]->ships[cls][shipId] = currShip;

	return;
}

void shoot_default(char command[], board_t** board, dim_t* dim, player_t** players, int playerId) {
	field_t target;
	int argc = sscanf(command, "%*s %d %d", &target.y, &target.x);

	assert(players[playerId]->shipPlaced <= get_fleet_size(players[playerId]));

	if (argc != 2) {
		handle_invalid_command(command, C_INVALID);
	}
	if (!check_field_on_board(dim, target)) {
		handle_invalid_command(command, C_FIELD_DOES_NOT_EXIST);
	}
	if (players[PLAYER_A]->shipPlaced + players[PLAYER_B]->shipPlaced != 
		get_fleet_size(players[PLAYER_A]) + get_fleet_size(players[PLAYER_B])) {
		handle_invalid_command(command, C_NOT_ALL_SHIPS_PLACED);
	}

	damage_ship(board, players, target);

	return;
}

void create_fleet(int fleetSize[], player_t* player) {
	assert(player != NULL);
	assert(sizeof(player->fleet[2]) == sizeof(int));

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		player->fleet[cls] = fleetSize[cls];
	}

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
			player->ships[cls][shipId].created = False;
			player->ships[cls][shipId].placed = False;
		}
	}

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < fleetSize[cls]; shipId++) {
			player->ships[cls][shipId].created = True;
			player->ships[cls][shipId].placed = False;
			for (int len = 0; len < cls; len++) {
				player->ships[cls][shipId].damaged[len] = 0;
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


	argc = sscanf(command, "%*s %c %d %d %d %d", &P, &fleetSize[5], &fleetSize[4], &fleetSize[3], &fleetSize[2]);

	int playerId = get_player_id(P);
	
	if (argc != 5) {
		handle_invalid_command(command, C_INVALID);
	}
	if(playerId == ERROR						|| // wrong ID
		fleetSize[2] < 0 || fleetSize[2] > 10	|| // wrong number of given ship class
		fleetSize[3] < 0 || fleetSize[3] > 10	||
		fleetSize[4] < 0 || fleetSize[4] > 10	||
		fleetSize[5] < 0 || fleetSize[5] > 10) {											

		handle_invalid_command(command, C_WRONG_ARGS);
	}

	create_fleet(fleetSize, players[playerId]);

	return;
}

int set_next_player(char command[]) {
	char P;
	int argc = sscanf(command, "%*s %c", &P);
	
	assert(argc == 1);
	
	if (P == 'A') {
		return PLAYER_A;
	}
	else if (P == 'B') {
		return PLAYER_B;
	}
	else {
		handle_invalid_command(command, C_WRONG_ARGS);
	}

	return C_INVALID;
}

int get_number_of_shots(int cls) {
	return cls;
}

void validate_shoot_extended(char command[], player_t** players, dim_t* dim, field_t target,
	ship_t* ship, int argc, int playerId, int cls, int shipId) 
{
	if (argc != 4) {
		handle_invalid_command(command, C_INVALID);
	}
	if (cls == ERROR || shipId >= 10 || players[playerId]->ships[cls][shipId].created == False) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (!check_field_on_board(dim, target)) {
		handle_invalid_command(command, C_FIELD_DOES_NOT_EXIST);
	}
	if (players[PLAYER_A]->shipPlaced + players[PLAYER_B]->shipPlaced !=
		get_fleet_size(players[PLAYER_A]) + get_fleet_size(players[PLAYER_B])) {
		handle_invalid_command(command, C_NOT_ALL_SHIPS_PLACED);
	}

	if (ship->damaged[1] == True) {
		handle_invalid_command(command, C_SHIP_CANNOT_SHOOT);
	}
	if (ship->shots == get_number_of_shots(cls)) {
		handle_invalid_command(command, C_TOO_MANY_SHOOTS);
	}

	field_t cannon = ship->head;
	cannon.x += dx[ship->direction];
	cannon.y += dy[ship->direction];

	if (get_shooting_range(cls) < get_dist(cannon, target)) {
		handle_invalid_command(command, C_SHOOTING_TOO_FAR);
	}

	return;
}

void shoot_extended(char command[], board_t** board, dim_t* dim, player_t** players, int playerId) {
	//SHOOT i C y x

	field_t target;
	char clsChar[MAX_SHIP_LENGTH];
	int shipId;
	int argc = sscanf(command, "%*s %d %s %d %d", &shipId, clsChar, &target.y, &target.x);

	int cls = get_class(clsChar);

	assert(players[playerId]->shipPlaced <= get_fleet_size(players[playerId]));

	ship_t ship = players[playerId]->ships[cls][shipId];

	validate_shoot_extended(command, players, dim, target, &ship, argc, playerId, cls, shipId);
	
	ship.shots++;

	players[playerId]->ships[cls][shipId] = ship;
	damage_ship(board, players, target);

	return;
}

void validate_spy_command(char command[], player_t** players, int playerId, int argc, int shipId) {
	if (argc != 3) {
		handle_invalid_command(command, C_INVALID);
	}
	if (shipId >= 10 || players[playerId]->ships[S_CAR][shipId].created == False) { // ship does not exist
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if (players[playerId]->ships[S_CAR][shipId].damaged[1] == True) { // cannon destroyed
		handle_invalid_command(command, C_CANNOT_SEND_PLANE);
	}
	if (players[playerId]->ships[S_CAR][shipId].spies == S_CAR) { // all spies sent
		handle_invalid_command(command, C_ALL_PLANES_SENT);
	}

	return;
}

void spy(char command[], board_t** board, dim_t* dim, player_t** players, int playerId) {
	int shipId;
	field_t target;
	int argc = sscanf(command, "%*s %d %d %d", &shipId, &target.y, &target.x);

	validate_spy_command(command, players, playerId, argc, shipId);

	players[playerId]->ships[S_CAR][shipId].spies++;

	for (int dir = -1; dir <= NW; dir++) {
		field_t newTarget = target;

		if (dir != -1) {
			newTarget.x += dx[dir];
			newTarget.y += dy[dir];
		}
		if (check_field_on_board(dim, newTarget) == False) {
			continue;
		}

		int spy = board[newTarget.y][newTarget.x].spy;

		if (spy == B_EMPTY) {
			spy = playerId;
		}
		else if (spy != playerId) {
			spy = B_VISIBLE_BOTH;
		}
		else {
			assert(spy == playerId);
		}

		assert(spy != B_EMPTY);
		board[newTarget.y][newTarget.x].spy = spy;
	}

	return;
}

bool is_ship_placed(int cls, int id, player_t* player) {
	for (int i = 0; i < MAX_SHIPS_NUMBER; i++) {
		if (player->ships[cls][i].created == True	&&
			player->ships[cls][i].placed == True	&&
			i == id) {
			return True;
		}
	}
	return False;
}
