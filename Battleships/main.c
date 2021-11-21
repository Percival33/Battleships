#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "commands.h"
#include "constans.h"




int main() {

	int quit = 0;
	int command;
	int activeCommand = 0; 

	while (!quit) {
		command = get_command_type();

		switch (command) {

			case STATE_C:
				handle_command(command, &activeCommand, STATE_C);
				break;

			case PLAYER_A_C:
				handle_command(command, &activeCommand, PLAYER_A_C);
				break;

			case PLAYER_B_C:
				handle_command(command, &activeCommand, PLAYER_B_C);
				break;

			case QUIT_C:
				handle_command(command, &activeCommand, QUIT_C);
				quit = 1;
				break;

			default:
				handle_invalid_command(command, &activeCommand);
				break;
		}
	}



	return 0;
}