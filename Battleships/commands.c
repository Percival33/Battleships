#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
		is_correct_command(command, "EXTENDED_SHIPS")) {
		return True;
	}
	return False;
}

bool is_player_type_command(char command[]) {
	if (is_correct_command(command, PLACE_SHIP_CHAR)	||
		is_correct_command(command, SHOOT_CHAR)			||
		is_correct_command(command, MOVE_CHAR)) {
		return True;
	}
	return False;
}

int get_command_type(char command[]) {

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

	else if (is_state_type_command(command)) {
		return C_STATE_TYPE;
	}


	else if(is_player_type_command(command)) {
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
	case PLAYER_A || PLAYER_B:
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
	}
	
	printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, exception);
	
	exit(1);
}

void handle_player_command(char command[], board_t** board, player_t** players, dim_t* dim, int playerId, int* shoots, int extendedShips) {
	
	if (is_correct_command(command, PLACE_SHIP_CHAR)) {
		place_ship(command, board, players[playerId], dim);
	}
	else if (is_correct_command(command, SHOOT_CHAR)) {
	
		if (extendedShips) {
			//shoot_extended(); TODO write function
			//*shoots++ ??
			return;
		}
	
		assert(*shoots == 0);
		if (*shoots == 1) {
			handle_invalid_command(command, C_INVALID);
		}
		*shoots += 1;
		shoot_default(command, board, dim, players, playerId);
	}
	else if (is_correct_command(command, MOVE_CHAR)) {
		move(command, board, dim, players[playerId]);
	}
	else {
		handle_invalid_command(command, C_INVALID);
	}
	return;
}

dim_t set_dim_size(char command[]) {
	int y;
	int x;
	int argc = sscanf(command, "%*s %d %d", &y, &x);

	if (argc != 2 || y <= 0 || x <= 0) {
		handle_invalid_command(command, C_INVALID);
	}
	
	dim_t dim;
	dim.ROWS = y;
	dim.COLS = x;
	return dim;
}

void set_board_size(char command[], board_t** board, dim_t* dim) {
	if (dim->ROWS != DEFAULT_COLS_NUMBER && dim->COLS != DEFAULT_COLS_NUMBER) {
		handle_invalid_command(command, C_INVALID);
	}
	board_free(board, dim);
	dim_t dim_local = set_dim_size(command); 
	dim->ROWS = dim_local.ROWS;
	dim->COLS = dim_local.COLS;
	board = board_init(dim);
	return;
}

void set_init_position(char command[], player_t** players, dim_t* dim) {
	//INIT_POSITION P y1 x1 y2 x2 
	int yL;
	int yR;
	int xL;
	int xR;
	char P;

	int argc = sscanf(command, "%*s %c %d %d %d %d", &P, &yL, &xL, &yR, &xR);

	int playerId = get_player_id(P);

	if (argc != 5) {
		handle_invalid_command(command, C_INVALID);
	}
	if (playerId == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if ((yL < 0			|| yL > dim->ROWS)	||
		(xL < 0			|| xL > dim->COLS)	||
		(xR < xL		|| yR < yL)			||
		(xR > dim->COLS	|| yR > dim->ROWS)) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}

	players[playerId]->rowLow = yL;
	players[playerId]->rowHigh = yR + 1;
	players[playerId]->colLow = xL;
	players[playerId]->colHigh = xR + 1;


	return;
}

void set_reef(char command[], board_t** board, dim_t* dim) {
	int y;
	int x;
	int argc = sscanf(command, "%*s %d %d", &y, &x);
	if (argc != 2) {
		handle_invalid_command(command, C_INVALID);
	}

	if (y < 0 || y >= dim->ROWS || x < 0 || x >= dim->COLS) {
		handle_invalid_command(command, C_REEF_NOT_ON_BOARD);
	}

	board[y][x].type = B_REEF;
	return;

}

void handle_state_commands(char command[], int *nextPlayer, board_t** board, player_t** players, dim_t* dim, int* extendedShips) {
	//assert(nextPlayer != NULL);
	if (is_correct_command(command, PRINT_CHAR)) {
		int x;
		int argc = sscanf(command, "%*s %d", &x);
		assert(argc == 1); 
		assert(x == 0 || x == 1);
		board_print(board, dim, x, players);
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
	}
	
	else if (is_correct_command(command, SHIP_CHAR)) {
		set_ship(command, board, players, dim);
	}
	
	else if (is_correct_command(command, "EXTENDED_SHIPS")) {
		*extendedShips = True;
	}
	
	else {
		handle_invalid_command(command, C_INVALID);
	}
	return;
}