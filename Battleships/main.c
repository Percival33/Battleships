#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ai.h"
#include "board.h"
#include "commands.h"
#include "constants.h"
#include "save.h"
#include "ship.h"
#include "player.h"
#include "vector.h"

#define debug False

void free_memory(board_t** board, dim_t* dim, player_t** players) {
	board_free(board, dim);
	player_free(players);
}


void handle_player_ends_turn(char command[], int commandId, player_t** players, int* activeCommandType,
	int* currentPlayer, int* nextPlayer, int* shots)
{
	
	if (*activeCommandType == commandId) {
		check_winner(players);
		*activeCommandType = C_NULL;
		assert(*currentPlayer == PLAYER_A || *currentPlayer == PLAYER_B);
		*nextPlayer = (*currentPlayer == PLAYER_A ? PLAYER_B : PLAYER_A);
		*currentPlayer = C_NULL;
		*shots = 0;
		clear_moved(players[commandId]);
		clear_shots(players[commandId]);
		clear_spies(players[commandId]);
	}
	else {
		handle_invalid_command(command, commandId);
	}
	return;
}

void handle_all_commands(int commandId, int* quit, int* activeCommandType, char command[], int* nextPlayer,
	board_t** board, player_t** players, dim_t* dim, int* extendedShips, vector_t* savedCommands, vector_t* reefs,
	int* seed, int* currentPlayer, int* shots, int* aiPlayer, int* aiMoved)
{

	switch (commandId) {
		case C_QUIT:
			*quit = 1;
			break;

		case C_STATE:
			if (*aiPlayer != ERROR && *aiMoved == False) {
				*aiMoved = True;
				srand(*seed);

				save_game_state(savedCommands, reefs, board, dim, players, nextPlayer, *extendedShips, seed, *aiPlayer);
				
				run_ai(savedCommands, reefs, board, dim, players, extendedShips, seed, nextPlayer, aiPlayer);
			}
			*activeCommandType = C_NULL;
			break;

		case C_STATE_TYPE:
			handle_state_commands(command, nextPlayer, board, players, dim, extendedShips, savedCommands, reefs, seed, aiPlayer);
			break;

		case C_PLAYER_TYPE:
			handle_player_command(command, board, players, dim, *currentPlayer, shots, *extendedShips);
			break;

		case PLAYER_A:
			if (*aiPlayer == PLAYER_B) {
				run_ai(savedCommands, reefs, board, dim, players, extendedShips, seed, nextPlayer, aiPlayer);
			}
			handle_player_ends_turn(command, commandId, players, activeCommandType, currentPlayer, nextPlayer, shots);
			break;

		case PLAYER_B:
			if (*aiPlayer == PLAYER_A) {
				run_ai(savedCommands, reefs, board, dim, players, extendedShips, seed, nextPlayer, aiPlayer);
			}
			handle_player_ends_turn(command, commandId, players, activeCommandType, currentPlayer, nextPlayer, shots);
			break;

		case C_INVALID:
			handle_invalid_command(command, C_INVALID);
			break;
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
	int extendedShips;
	int seed; 
	int shots;
	int aiPlayer;
	int aiMoved;
	
	extendedShips = False;
	quit = False;
	event = C_NULL;
	activeCommandType = C_NULL;
	nextPlayer = PLAYER_A;
	seed = DEFAULT_SEED;
	shots = 0;
	aiPlayer = ERROR;
	aiMoved = False;
	
	dim_t* dim = dim_init(DEFAULT_ROWS_NUMBER, DEFAULT_COLS_NUMBER);
	player_t** players = init_players(dim);
	board_t** board = board_init(dim);
	vector_t savedCommands, reefs;

	init(&savedCommands);
	init(&reefs);

	set_default_fleet(players);

	char command[MAX_COMMAND_LENGTH];
	
	//srand(time(NULL));

	while (!quit) {
		if (fgets(command, MAX_COMMAND_LENGTH - 2, stdin) == NULL)
			break;

		if (command[0] == '\n') {
			continue;
		}


		commandId = get_command_type(command, activeCommandType);

		if (activeCommandType == C_NULL) {
			if ((commandId == PLAYER_A || commandId == PLAYER_B)) {
				if (nextPlayer == PLAYER_A && commandId == PLAYER_A) {
					activeCommandType = PLAYER_A;
					currentPlayer = PLAYER_A;
					nextPlayer = PLAYER_B;
					continue;
				}
				else if (nextPlayer == PLAYER_B && commandId == PLAYER_B) {
					activeCommandType = PLAYER_B;
					currentPlayer = PLAYER_B;
					nextPlayer = PLAYER_A;
					continue;
				}
				else {
					handle_invalid_command(command, PLAYER_A);
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

			handle_all_commands(commandId, &quit, &activeCommandType, command, &nextPlayer, board,
				players, dim, &extendedShips, &savedCommands, &reefs, &seed, &currentPlayer, &shots, &aiPlayer, &aiMoved);
			
		}
			
	}

	free_memory(board, dim, players);

	return 0;
}
