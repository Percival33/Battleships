#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "commands.h"
#include "constants.h"
#include "ship.h"
#include "player.h"

#define debug False

int main() {

	int quit = 0;
	int event = C_NULL;
	int commandId;
	int activeCommandType = C_NULL;

	dim_t dim = { 21, 10 }; // init { ROWS, COLS }

	char command[100];

	board_t** board = board_init(&dim);


	player_t playerA = { 0, 9 }; // init playerA rows
	player_t playerB = { 11, 20 }; // init playerB rows

	field_t field = { 8 ,5 };


	set_fleet("SET_FLEET 2 1 3 7", &playerA);
	printf("strzal: %d\n", shoot(board, &dim, &playerA, "SHOOT 10 5"));

	if (debug) {
		set_fleet("SET_FLEET 2 1 3 7", &playerB);
		place_ship(board, "PLACE_SHIP 12 4 N 1 DES", &playerA);
		printf("czy DES: %d", get_class("DES"));
		printf("czy {field} u playerA: %d\n", board_inside(&dim, &field));
		printf("czy {field} u playerB: %d\n", board_inside(&dim, &field));
	}
	
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

	fleet_free(fleet);
	board_free(board, &dim);

	return 0;
}