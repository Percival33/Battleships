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

void set_default_fleet(player_t** players) {
	set_fleet("SET_FLEET A 1 2 3 4", players);
	set_fleet("SET_FLEET B 1 2 3 4", players);
	return;
}

player_t** init_players(dim_t* dim) {
	player_t** players = (player_t**)malloc(2 * sizeof(player_t*));
	assert(players != NULL);

	players[PLAYER_A] = player_init(0, (dim->ROWS / 2), PLAYER_A);
	players[PLAYER_B] = player_init((dim->ROWS / 2) + 1, dim->ROWS, PLAYER_B);

	return players;
}

void free_memory(board_t** board, dim_t* dim, player_t** players) {
	fleet_free();
	board_free(board, dim);
	player_free(players);
}

void check_winner(player_t** players) {
	if (players[PLAYER_A]->shipPlaced + players[PLAYER_B]->shipPlaced !=
		get_fleet_size(players[PLAYER_A]) + get_fleet_size(players[PLAYER_B]))
		return;
	int AReamaining = get_remaining_parts(players[PLAYER_A]);
	int BReamaining = get_remaining_parts(players[PLAYER_B]);

	if (AReamaining == 0) {
		printf("B won\n");
		exit(0);
	}
	if (BReamaining == 0) {
		printf("A won\n");
		exit(0);
	}
	return;
}

int main() {

	int quit;
	int event;
	int commandId;
	int activeCommandType;
	int nextPlayer;
	int currentPlayer;

	quit = 0;
	event = C_NULL;
	activeCommandType = C_NULL;
	nextPlayer = C_PLAYER_A;
	
	dim_t* dim = dim_init(DEFAULT_ROWS_NUMBER, DEFAULT_COLS_NUMBER);
	player_t** players = init_players(dim);
	board_t** board = board_init(dim);
	
	set_default_fleet(players);

	char command[MAX_COMMAND_LENGTH];
	int temp;

	while (!quit) {
		if (fgets(command, MAX_COMMAND_LENGTH - 2, stdin) == NULL)
			break;

		if (command[0] == '\n') {
			continue;
		}

		commandId = get_command_type(command);

		if (activeCommandType == C_NULL) {
			if ((commandId == C_PLAYER_A || commandId == C_PLAYER_B)) {
				if (nextPlayer == C_PLAYER_A && commandId == C_PLAYER_A) {
					activeCommandType = C_PLAYER_A;
					currentPlayer = PLAYER_A;
					nextPlayer = C_PLAYER_B;
					continue;
				}
				else if (nextPlayer == C_PLAYER_B && commandId == C_PLAYER_B) {
					activeCommandType = C_PLAYER_B;
					currentPlayer = PLAYER_B;
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
					handle_state_commands(command, &nextPlayer, board, players, dim);
					break;

				case C_PLAYER_TYPE:
					handle_player_command(command, board, players, dim, currentPlayer);
					break;

				case C_PLAYER_A:
					if (activeCommandType == C_PLAYER_A) {
						check_winner(players);
						activeCommandType = C_NULL;
						currentPlayer = C_NULL;
					}
					else {
						handle_invalid_command(command, C_PLAYER_A);
					}
					break;

				case C_PLAYER_B:
					if (activeCommandType == C_PLAYER_B) {
						check_winner(players);
						activeCommandType = C_NULL;
						currentPlayer = C_NULL;
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
	free_memory(board, dim, players);
	
	return 0;
}