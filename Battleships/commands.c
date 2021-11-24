#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "constants.h"
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
		printf("INVALID OPERATION \"%.*s \": %s\n", len-1, command, OTHER_PLAYER_EXPECTED_CHAR);
	}
	else if (errorType == C_INVALID) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, WRONG_COMMAND_CHAR);
	}
	
	exit(1);
}

int handle_player_command(char command[], int activeCommandType) {
	if (is_correct_command(command, PLACE_SHIP_CHAR)) {
		
	}
	else if (is_correct_command(command, SHOOT_CHAR)) {
		
	}
	else {
		handle_invalid_command(command, activeCommandType); // TODO: pass accurate args
	}
	return;
}

void handle_state_commands(char command[], int activeCommandType, int* nextPlayer) {
	if (is_correct_command(command, PRINT_CHAR)) {

	}
	else if (is_correct_command(command, SET_FLEET_CHAR)) {

	}
	else if (is_correct_command(command, NEXT_PLAYER_CHAR)) {
		*nextPlayer = set_next_player(command);
	}
	else {
		handle_invalid_command(command, activeCommandType); // TODO: pass accurate args
	}
	return;
}