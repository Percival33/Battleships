#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "commands.h"
#include "constants.h"

const char STATE[] = "[state]";
const char PLAYER_A[] = "[playerA]";
const char PLAYER_B[] = "[playerB]";
const char PLACE_SHIP[] = "PLACE_SHIP";

const char SHIP_NOT_ON_POSITION[] = "NOT IN STARTING POSITION";
const char SHIP_PRESENT[] = "SHIP ALREADY PRESENT";
const char SHIP_ALL_SET[] = "ALL SHIPS OF THE CLASS ALREADY SET";

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

void handle_invalid_command(char command[], char tag[], char reason[]) {
	printf("INVALID OPERATION %s SPECIFIED: %s\n", tag, reason);
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
	else {
		printf("invalid command czy cos?\n");
	}
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

int handle_command(char command[]) {
	return -1;
}

int validate_place_ship(char command[], int ROWS, int COLS) {

	if (is_correct_command(command, PLACE_SHIP)) {
		int id = strlen(PLACE_SHIP) + 1;
		int y = get_number(command, &id);
		int x = get_number(command, &id);
		int dir;

		char dirChar = command[id];

		assert(command[id + 1] == ' ');

		id += 2;		// add 2 due to name of dir and one space

		switch (dirChar) {
		case 'N':
			dir = N;
			break;
		case 'E':
			dir = E;
			break;
		case 'S':
			dir = S;
			break;
		case 'W':
			dir = W;
			break;

		default:
			dir = -1;
			break;
		}

		if (!(0 <= y && y < ROWS) || !(0 <= x && x < COLS) || dir == -1) { // head of ship not in starting point

			handle_invalid_command(command, PLACE_SHIP, SHIP_NOT_ON_POSITION);
			return C_INVALID; // TODO: print that command is invalid

		}

		int shipId = get_number(command, &id);
		char shipTypeChar[4] = { command[id], command[id + 1], command[id + 2], '\0' };
		int shipType;

		if (is_correct_command(shipTypeChar, "CAR")) {
			shipType = S_CAR;
		}
		else if (is_correct_command(shipTypeChar, "BAT")) {
			shipType = S_BAT;
		}
		else if (is_correct_command(shipTypeChar, "CRU")) {
			shipType = S_CRU;
		}
		else if (is_correct_command(shipTypeChar, "DES")) {
			shipType = S_DES;
		}
		else {
			shipType = -1;
		}

		assert(dir != -1);
		assert(1 <= shipId <= 10); // TODO: min, max id of shiptype
		assert(shipType != -1);


		printf("x: %d y:%d dir: %c id: %d type: %d \n", x, y, dirChar, shipId, shipType);

		return True;
	}
	return False;
}

