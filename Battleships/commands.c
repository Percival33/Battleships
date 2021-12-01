#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "constants.h"
#include "board.h"
#include "player.h"
#include "commands.h"

int is_correct_command(char command[], char type[]) {
	if (strncmp(command, type, strlen(type)) == 0) {
		return True;
	}
	return False;
}

int get_command_type(char command[]) {

	if (is_correct_command(command, STATE_CHAR)) {
		return C_STATE;
	}
	else if (is_correct_command(command, PLAYER_A_CHAR)) {
		return C_PLAYER_A;
	}
	else if (is_correct_command(command, PLAYER_B_CHAR)) {
		return C_PLAYER_B;
	}

	else if (is_correct_command(command, "Q")) {
		return C_QUIT;
	}

	else if ((is_correct_command(command, PRINT_CHAR)) ||
		(is_correct_command(command, SET_FLEET_CHAR)) ||
		(is_correct_command(command, NEXT_PLAYER_CHAR))) {
		return C_STATE_TYPE;
	}


	else if (is_correct_command(command, PLACE_SHIP_CHAR) ||
			(is_correct_command(command, SHOOT_CHAR))) {
		return C_PLAYER_TYPE;
	}
	else {
		return C_INVALID;
	}
}

void handle_invalid_command(char command[], int errorType) {

	int len = strlen(command);

	if (errorType == C_PLAYER_A || errorType == C_PLAYER_B) {
		printf("INVALID OPERATION \"%.*s \": %s\n", len - 1, command, OTHER_PLAYER_EXPECTED_CHAR);
	}
	else if (errorType == C_INVALID) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, WRONG_COMMAND_CHAR);
	}
	else if (errorType == C_NOT_ALL_SHIPS_PLACED) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, SHIP_NOT_ALL_SHIPS_PLACED_CHAR);
	}
	else if (errorType == C_FIELD_DOES_NOT_EXIST) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, FIELD_DOES_NOT_EXIST_CHAR);
	}
	else if (errorType == C_NOT_IN_STARTING_POSITION) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, NOT_IN_STARTING_POSITION_CHAR);
	}
	else if (errorType == C_SHIP_ALREADY_PRESENT) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, SHIP_ALREADY_PRESENT_CHAR);
	}
	else if (errorType == C_ALL_SHIPS_OF_THE_CLASS_ALREADY_PRESENT) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command,
				ALL_SHIPS_OF_THE_CLASS_ALREADY_PRESENT_CHAR);
	}
	

	
	exit(1);
}

void handle_player_command(char command[], board_t** board, player_t** players, dim_t* dim, int playerId) {
	
	if (is_correct_command(command, PLACE_SHIP_CHAR)) {
		place_ship(command, board, players[playerId], dim);
	}
	else if (is_correct_command(command, SHOOT_CHAR)) {
		shoot(command, board, dim, players, playerId); //FIXME check if it works
	}
	else {
		handle_invalid_command(command, C_INVALID); // TODO: pass accurate args
	}
	return;
}

void handle_state_commands(char command[], int *nextPlayer, board_t** board, player_t** players, dim_t* dim) {
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
	else {
		handle_invalid_command(command, C_INVALID); // TODO: pass accurate args
	}
	return;
}