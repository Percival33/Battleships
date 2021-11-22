#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "board.h"
#include "commands.h"
#include "constants.h"

int main() {

	int quit = 0;
	int event;
	int commandId;
	int activeCommand = 0;

	const int ROWS = 21;
	const int COLS = 10;
	

	char command[100];

	board_t** board = board_init(ROWS, COLS);
	
	//board_print(board, ROWS, COLS);
	//printf("liczba: %d\n", get_number("PLACE_SHIP 12 4 N 1 CAT", strlen("PLACE_SHIP")+1));
	printf("liczba: %d\n", validate_place_ship("PLACE_SHIP 12 4 N 1 DES", ROWS, COLS));
	//gets(command);
	//printf("liczba: %d\n", validate_place_ship(command, ROWS, COLS));

	while (!quit) {

		gets(command);

		commandId = get_command_type(&command);

		switch (commandId) {

		case C_STATE:
			event = handle_command_type(&command, &activeCommand, C_STATE);
			if (event == C_IN) {
				printf("command: %d type: %d\n", C_STATE, C_IN);
			}
			else if (event == C_OUT) {
				printf("command: %d type: %d\n", C_STATE, C_IN);
			}
			break;

		case C_PLAYER_A:

			event = handle_command_type(command, &activeCommand, C_PLAYER_A);
			if (event == C_IN) {

			}
			else if (event == C_OUT) {

			}
			break;

		case C_PLAYER_B:
			event = handle_command_type(command, &activeCommand, C_PLAYER_B);
			if (event == C_IN) {

			}
			else if (event == C_OUT) {

			}
			break;

		case C_QUIT:
			quit = 1;
			break;

		default:
			printf("INPUT CORRECT COMMAND\n");
			break;
		}
	}

	board_free(board, ROWS);

	return 0;
}