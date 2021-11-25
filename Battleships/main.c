#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "board.h"
#include "commands.h"
#include "constants.h"
#include "ship.h"
#include "player.h"

#define debug False

int main() {

	int quit;
	int event;
	int commandId;
	int activeCommandType;
	int nextPlayer;

	quit = 0;
	event = C_NULL;
	activeCommandType = C_NULL;
	nextPlayer = C_PLAYER_A;
	
	dim_t* dim = dim_init(21, 10);

	player_t** player;
	player = (player_t**)malloc(2 * sizeof(player_t*));
	assert(player != NULL);

	player[PLAYER_A] = player_init(0, (dim->ROWS/2), PLAYER_A);
	player[PLAYER_B] = player_init((dim->ROWS/2) + 1, dim->ROWS, PLAYER_B);

	board_t** board = board_init(dim);
	
	set_fleet("SET_FLEET A 1 2 3 4", player);
	set_fleet("SET_FLEET B 1 2 3 4", player);

	char command[101];
	int temp;

	while (!quit) {
		if (fgets(command, 100, stdin) == NULL)
			break;

		if (command[0] == '\n') {
			continue;
		}

		commandId = get_command_type(command);

		if (activeCommandType == C_NULL) {
			if ((commandId == C_PLAYER_A || commandId == C_PLAYER_B)) {
				if (nextPlayer == C_PLAYER_A && commandId == C_PLAYER_A) {
					activeCommandType = C_PLAYER_A;
					nextPlayer = C_PLAYER_B;
					continue;
				}
				else if (nextPlayer == C_PLAYER_B && commandId == C_PLAYER_B) {
					activeCommandType = C_PLAYER_B;
					nextPlayer = C_PLAYER_A;
					continue;
				}
				else {
					handle_invalid_command(command, C_PLAYER_A);
				}
			}
			else if (commandId == C_STATE) {
				activeCommandType = C_STATE;
				continue;
			}
			else {
				handle_invalid_command(command, C_INVALID);
			}
		}
		else {

			switch (commandId) {
				case C_QUIT:
					quit = 1;
					break;

				case C_STATE:
					activeCommandType = C_NULL;
					break;

				case C_STATE_TYPE:
					handle_state_commands(command, &nextPlayer, board, player, dim);
					break;

				case C_PLAYER_TYPE:
					handle_player_command(command, activeCommandType);
					break;

				case C_PLAYER_A:
					if (activeCommandType == C_PLAYER_A) {
						activeCommandType = C_NULL;
					}
					else {
						handle_invalid_command(command, C_PLAYER_A);
					}
					break;

				case C_PLAYER_B:
					if (activeCommandType == C_PLAYER_B) {
						activeCommandType = C_NULL;
					}
					else {
						handle_invalid_command(command, C_PLAYER_B);
					}
					break;

				case C_INVALID:
					handle_invalid_command(command, C_INVALID);
					break;
			}
		}
			
	}

	fleet_free();
	board_free(board, dim);
	free(player[PLAYER_A]);
	free(player[PLAYER_B]);
	free(player);
	return 0;
}