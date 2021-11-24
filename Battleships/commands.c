#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "commands.h"
#include "constants.h"



int is_correct_command(char command[], char type[]) {
	if (strncmp(command, type, strlen(type)) == 0) {
		return True;
	}
	return False;
}

int get_command_type(char command[]) {

	if (is_correct_command(command, STATE)) {
		return C_STATE;
	}
	else if (is_correct_command(command, PLAYER_A)) {
		return C_PLAYER_A;
	}
	else if (is_correct_command(command, PLAYER_B)) {
		return C_PLAYER_B;
	}

	else if (is_correct_command(command, "Q")) {
		return C_QUIT;
	}

	else if ((is_correct_command(command, PRINT)) ||
		(is_correct_command(command, SET_FLEET)) ||
		(is_correct_command(command, NEXT_PLAYER))) {
		return C_STATE_TYPE;
	}


	else if (is_correct_command(command, PLACE_SHIP) ||
			(is_correct_command(command, SHOOT))) {
		return C_PLAYER_TYPE;
	}
	else {
		return C_INVALID;
	}
}

void print_invalid_command(char command[], char reason[]) {

}

void handle_invalid_command(char command[], int errorType) {

	int len = strlen(command);

	if (errorType == C_PLAYER_A || errorType == C_PLAYER_B) {
		printf("INVALID OPERATION \"%.*s \": %s\n", len-1, command, OTHER_PLAYER_EXPECTED);
	}
	else if (errorType == C_INVALID) {
		printf("INVALID OPERATION \"%.*s\": %s\n", len - 1, command, WRONG_COMMAND);
	}
	
	exit(1);
}

int handle_player_command(char command[], int activeCommandType) {
	if (is_correct_command(command, PLACE_SHIP)) {
		
	}
	else if (is_correct_command(command, SHOOT)) {
		
	}
	else {
		handle_invalid_command(command, activeCommandType); // TODO: pass accurate args
	}
	return -1;
}

void handle_state_commands(char command[], int activeCommandType) {

}