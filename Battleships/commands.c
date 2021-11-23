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
	else {
		return C_INVALID;
	}
}

void print_invalid_command(char command[], char reason[]) {

}

void handle_invalid_command(char command[], int activeCommandType) {
	//printf("INVALID OPERATION \"%s \": %s\n", command, reason);
	
	/*
				switch (activeCommandType) {
					case C_PLAYER_A || C_PLAYER_B:
						handle_invalid_command(command, OTHER_PLAYER_EXPECTED);
						break;
					default:
						printf("main cos dupa\n");
						break;
				}
	*/
	
	switch (activeCommandType) {
			
	}
	//print_invalid_command(command);
	exit(1);
	return;
}

int handle_command_type(char command[], int* activeCommand, int commandType) {
	
	if (*activeCommand == commandType) {
		*activeCommand = C_NULL;
		return C_OUT;
	}
	
	else if (*activeCommand == C_NULL) {
		*activeCommand = commandType;
		return C_IN;
	}
	printf("zla komenda jakas\n");
	return C_INVALID;
}

int get_number(char command[], int* id) {
	int spaces = 1;
	char number[10];
	int i = 0;

	while (*id < strlen(command) && spaces < 2) {
		if (command[*id] == ' ')
			spaces++;
		number[i++] = command[(*id)++];
	}
	number[i] = '\0';
	printf("index: %d\n", *id);
	return atoi(number);
}
