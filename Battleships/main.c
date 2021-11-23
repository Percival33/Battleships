#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "commands.h"
#include "constants.h"
#include "ship.h"
#include "player.h"

int main() {

	int quit = 0;
	int event = C_NULL;
	int commandId;
	int activeCommandType = C_NULL;

	const int ROWS = 21;
	const int COLS = 10;
	

	char command[100];

	board_t** board = board_init(ROWS, COLS);
	
	player_t playerA = { 0, 9 };
	player_t playerB = { 11, 20 };

//	place_ship(board, "PLACE_SHIP 12 4 N 1 DES", &playerA);
//	printf("czy DES: %d", get_class("DES"));
	
	while (!quit) {

		gets(command);

		if (activeCommandType != C_NULL) { // type of commands is specified
			switch (activeCommandType) {
				case C_STATE:
					// handles and validates all state commands 
					break;
				case C_PLAYER_A:
					// handles and validates all player_a commands
					break;
				case C_PLAYER_B:
					// handles and validates all player_b commands
					break;
			}
		}
		else {
			commandId = get_command_type(&command);
			
			if (commandId == C_QUIT) {
				quit = 1;
				continue;
			}

			//event = handle_command_type(command, &activeCommandType, commandId);
			//TODO: merge get_command_type with handle_command_type
		
			if (event == C_INVALID) {
				handle_invalid_command(command, activeCommandType);
			}
			else {
				printf("event: %d acitveCommand: %d\n", event, activeCommandType);
			}

		}	
	}

	board_free(board, ROWS);

	return 0;
}