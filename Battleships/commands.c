#define _CRT_SECURE_NO_WARNINGS
#define STOS (errorType == PLAYER_A || errorType == PLAYER_B)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vector.h"
#include "save.h"
#include "constants.h"
#include "board.h"
#include "player.h"
#include "commands.h"
#include "ship.h"

bool is_correct_command(char command[], char type[]) {
	if (strncmp(command, type, strlen(type)) == 0) {
		return True;
	}
	return False;
}

bool is_state_type_command(char command[]) {
	if (is_correct_command(command, PRINT_CHAR)			||
		is_correct_command(command, SET_FLEET_CHAR)		||
		is_correct_command(command, NEXT_PLAYER_CHAR)	||
		is_correct_command(command, BOARD_SIZE_CHAR)	||
		is_correct_command(command, INIT_POSITION_CHAR) ||
		is_correct_command(command, REEF_CHAR)			||
		is_correct_command(command, SHIP_CHAR)			||
		is_correct_command(command, "EXTENDED_SHIPS")	||
		is_correct_command(command, "SRAND")			||
		is_correct_command(command, "SAVE")) {
		return True;
	}
	return False;
}

bool is_player_type_command(char command[]) {
	if (is_correct_command(command, PLACE_SHIP_CHAR)	||
		is_correct_command(command, SHOOT_CHAR)			||
		is_correct_command(command, MOVE_CHAR)			||
		is_correct_command(command, PRINT_CHAR)			||
		is_correct_command(command, SPY_CHAR)) {
		return True;
	}
	return False;
}

int get_command_type(char command[], int activeCommandType) {

	if (is_correct_command(command, STATE_CHAR)) {
		return C_STATE;
	}

	else if (is_correct_command(command, PLAYER_A_CHAR)) {
		return PLAYER_A;
	}
	else if (is_correct_command(command, PLAYER_B_CHAR)) {
		return PLAYER_B;
	}

	else if (is_correct_command(command, "Q")) {
		return C_QUIT;
	}

	else if (activeCommandType == C_STATE && is_state_type_command(command)) {
		return C_STATE_TYPE;
	}
	else if ((activeCommandType == PLAYER_A || activeCommandType == PLAYER_B)	&&
			is_player_type_command(command)) { // if player inputs a command and command is correct for a player
		return C_PLAYER_TYPE;
	}
	else {
		return C_INVALID;
	}
}

void handle_invalid_command(char command[], int errorType) { //TODO fix readability

	int len = strlen(command);
	char* exception = "lolz Nie Dziala";
	switch (errorType) {
	case PLAYER_A:
		exception = OTHER_PLAYER_EXPECTED_CHAR;
		break;
	case PLAYER_B:
		exception = OTHER_PLAYER_EXPECTED_CHAR;
		break;

	case C_INVALID:
		exception = WRONG_COMMAND_CHAR;
		break;

	case C_NOT_ALL_SHIPS_PLACED:
		exception = SHIP_NOT_ALL_SHIPS_PLACED_CHAR;
		break;

	case C_WRONG_ARGS:
		exception = "WRONG ARGUMENTS";
		break;

	case C_FIELD_DOES_NOT_EXIST:
		exception = FIELD_DOES_NOT_EXIST_CHAR;
		break;

	case C_NOT_IN_STARTING_POSITION:
		exception = NOT_IN_STARTING_POSITION_CHAR;
		break;

	case C_SHIP_ALREADY_PRESENT:
		exception = SHIP_ALREADY_PRESENT_CHAR;
		break;

	case C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET:
		exception = ALL_SHIPS_OF_THE_CLASS_ALREADY_SET_CHAR;
		break;

	case C_REEF_NOT_ON_BOARD:
		exception = REEF_IS_NOT_PLACED_ON_BOARD_CHAR;
		break;

	case C_PLACING_SHIP_TOO_CLOSE:
		exception = PLACING_SHIP_TOO_CLOSE_TO_OTHER_SHIP_CHAR;
		break;

	case C_PLACING_SHIP_ON_REEF:
		exception = PLACING_SHIP_ON_REEF_CHAR;
		break;

	case C_SHIP_MOVED_ALREADY:
		exception = SHIP_MOVED_ALREADY_CHAR;
		break;

	case C_SHIP_WENT_FROM_BOARD:
		exception = SHIP_WENT_FROM_BOARD_CHAR;
		break;
	case C_SHIP_CANNOT_SHOOT:
		exception = SHIP_CANNOT_SHOOT_CHAR;
		break;
	case C_TOO_MANY_SHOOTS:
		exception = TOO_MANY_SHOOTS_CHAR;
		break;
	case C_SHOOTING_TOO_FAR:
		exception = SHOOTING_TOO_FAR_CHAR;
		break;
	case C_CANNOT_SEND_PLANE:
		exception = CANNOT_SEND_PLANE_CHAR;
		break;
	}
	if (STOS) {
		printf("INVALID OPERATION \"%.*s \": %s\n", len - 1, command, exception);		// (-.-)
	}
	else {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, exception);
	}

	exit(1);
}

void handle_player_command(char command[], board_t** board, player_t** players, dim_t* dim, int playerId, int* shots, int extendedShips) {
	
	if (is_correct_command(command, PLACE_SHIP_CHAR)) {
		place_ship(command, board, players[playerId], dim);
	}
	else if (is_correct_command(command, SHOOT_CHAR)) {
	
		if (extendedShips) {
			shoot_extended(command, board, dim, players, playerId);
			return;
		}
	
		assert(*shots == 0);
		if (*shots == 1) {
			handle_invalid_command(command, C_INVALID);
		}
		shoot_default(command, board, dim, players, playerId);
		*shots += 1;
	}
	else if (is_correct_command(command, SPY_CHAR)) {
		spy(command, board, dim, players, playerId);
		*shots += 1;
	}
	else if (is_correct_command(command, MOVE_CHAR)) {
		move(command, board, dim, players[playerId], extendedShips);
	}
	else if (is_correct_command(command, PRINT_CHAR)) {
		int x;
		int argc = sscanf(command, "%*s %d", &x);
		assert(argc == 1);
		assert(x == 0 || x == 1);
		player_board_print(board, dim, players, playerId, x, extendedShips);
	}
	else {
		handle_invalid_command(command, C_INVALID);
	}
	return;
}

void handle_state_commands(char command[], int *nextPlayer, board_t** board,
	player_t** players, dim_t* dim, int* extendedShips, vector_t* v, vector_t* reefs, int* seed) {

	if (is_correct_command(command, PRINT_CHAR)) {
		int x;
		int argc = sscanf(command, "%*s %d", &x);
		assert(argc == 1); 
		assert(x == 0 || x == 1);
		state_board_print(board, dim, x, players);
	}

	else if (is_correct_command(command, SET_FLEET_CHAR)) {
		set_fleet(command, players);
	}
	
	else if (is_correct_command(command, NEXT_PLAYER_CHAR)) {
		*nextPlayer = set_next_player(command);
	}
	
	else if (is_correct_command(command, BOARD_SIZE_CHAR)) {
		set_board_size(command, board, dim);
	}
	
	else if (is_correct_command(command, INIT_POSITION_CHAR)) {
		set_init_position(command, players, dim);
	}
	
	else if (is_correct_command(command, REEF_CHAR)) {
		set_reef(command, board, dim);
		push_back(reefs, command);
	}
	
	else if (is_correct_command(command, SHIP_CHAR)) {
		set_ship(command, board, players, dim);
	}
	
	else if (is_correct_command(command, "EXTENDED_SHIPS")) {
		*extendedShips = True;
	}

	else if (is_correct_command(command, "SRAND")) {
		assert(sizeof(seed) == sizeof(int*));
		int argc = sscanf(command, "%*s %d", seed);
		assert(sizeof(*seed) == sizeof(int));
		srand(*seed);
	}

	else if (is_correct_command(command, "SAVE")) {
		save_geme_state(v, reefs, board, dim, players, nextPlayer, extendedShips, seed);
	}
	
	else {
		handle_invalid_command(command, C_INVALID);
	}
	return;
}