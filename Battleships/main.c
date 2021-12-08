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

void set_init_values(int* extendedShips, int* quit, int* activeCommandType, 
	int* nextPlayer, int* seed, int* shots, int* aiPlayer, int* aiMoved)
{
	*extendedShips = False;
	*quit = False;
	*activeCommandType = C_NULL;
	*nextPlayer = PLAYER_A;
	*seed = DEFAULT_SEED;
	*shots = 0;
	*aiPlayer = ERROR;
	*aiMoved = False;

	return;
}


int main() {

	int quit;
	int commandId;
	int activeCommandType;
	int nextPlayer;
	int currentPlayer;
	int extendedShips;
	int seed; 
	int shots;
	int aiPlayer;
	int aiMoved;
	
	set_init_values(&extendedShips, &quit, &activeCommandType, &nextPlayer, &seed,
		&shots, &aiPlayer, &aiMoved);
	
	
	dim_t* dim = dim_init(DEFAULT_ROWS_NUMBER, DEFAULT_COLS_NUMBER);
	player_t** players = init_players(dim);
	board_t** board = board_init(dim);
	vector_t savedCommands, reefs;

	init(&savedCommands);
	init(&reefs);

	set_default_fleet(players);

	char command[MAX_COMMAND_LENGTH];
	
	while (!quit) {
		if (fgets(command, MAX_COMMAND_LENGTH - 2, stdin) == NULL)
			break;

		if (command[0] == '\n') {
			continue;
		}

		commandId = get_command_type(command, activeCommandType);

		if (activeCommandType == C_NULL) {
			if ((commandId == PLAYER_A || commandId == PLAYER_B)) {
				handle_changing_player_type(command, &nextPlayer, &commandId,
					&currentPlayer, &activeCommandType);
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
			if (strncmp(command, "BOARD_SIZE", strlen("BOARD_SIZE")) == 0) {
				if (dim->ROWS != DEFAULT_COLS_NUMBER && dim->COLS != DEFAULT_COLS_NUMBER) {
					handle_invalid_command(command, C_INVALID);
				}
				board_free(board, dim);
				dim_t dim_local = set_dim_size(command);
				dim->ROWS = dim_local.ROWS;
				dim->COLS = dim_local.COLS;
				board = board_init(dim);
			}
			else
				handle_all_commands(commandId, &quit, &activeCommandType, command, &nextPlayer, board,
				players, dim, &extendedShips, &savedCommands, &reefs, &seed, &currentPlayer, &shots, &aiPlayer, &aiMoved);
			
		}
	}

	free_memory(board, dim, players);

	return 0;
}
