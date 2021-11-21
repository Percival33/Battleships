#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "constans.h"


const char STATE[] = "[state]";
const char PLAYER_A[] = "[playerA]";
const char PLAYER_B[] = "[playerB]";

int is_correct_command(char command[], char type[]) {
	if (strncmp(command, type, strlen(type)) == 0) {
		return True;
	}
	return False;
}

void handle_invalid_command(char command[], int* activeCommand) {
	printf("INVALID COMMAND:\t %d\n", *activeCommand);
	return;
}

void handle_command(char command[], int* activeCommand, int type) {
	if (*activeCommand == type) {
		*activeCommand = 0;
		printf("%d OUT\n", type);
	}
	else if (*activeCommand == 0) {
		*activeCommand = type;
		printf("%d IN\n", type);
	}
	else {
		handle_invalid_command(command, activeCommand);
	}
	return;
}


int get_command_type() {
	char command[100];
	
	gets(command);

	if (is_correct_command(command, STATE)) {
		return STATE_C;
	}
	else if (is_correct_command(command, PLAYER_A)) {
		return PLAYER_A_C;
	}
	else if (is_correct_command(command, PLAYER_B)) {
		return PLAYER_B_C;
	}
	else if (is_correct_command(command, "Q")) {
		return QUIT_C;
	}
	else {
		return INVALID_C;
	}
}