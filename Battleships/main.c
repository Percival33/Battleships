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

void go_default(player_t* playerA, player_t* playerB) {
	set_fleet("", playerA);
	set_fleet("", playerB);
	return;
}

void board_print(board_t** board, dim_t* dim, player_t* playerA, player_t* playerB) {
	const int COLS = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;
	//	const int ROW_LOW = playerA->rowLow;
//	const int ROW_HIGH = playerA->rowHigh;

	for (int i = ROW_LOW; i < ROW_HIGH; i++) {
		for (int j = 0; j < COLS; j++) {
			if (board[i][j].type == B_EMPTY || board[i][j].type == B_BAN) {
				printf(" ");
			}
			else if (board[i][j].type == B_TAKEN) {
				printf("+");
			}
			else if (board[i][j].type == B_DESTROYED) {
				printf("x");
			}
			//printf("{%d,%d} = %d ", i, j, board[i][j].type);
		}
		printf("\n");
	}

	//TODO: print A and B remaining
	//TODO: get fleet of playerA and playerB and count reamaining parts
	int AReamaining = get_remaining_parts(playerA);
	int BReamaining = get_remaining_parts(playerB);


	printf("Arem: %d Brem: %d\n", AReamaining, BReamaining);

	return;

}

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

	player_t* playerA = player_init(0, dim->ROWS/2);
	player_t* playerB = player_init(dim->ROWS/2 + 1, dim->ROWS);
	board_t** board = board_init(dim);
	
	go_default(playerA, playerB);
	char command[101];

	while (!quit) {

		fgets(command, 100, stdin);

		commandId = get_command_type(command);

		switch (activeCommandType) {
			case C_QUIT:
				quit = 1;
				break;



			case C_STATE:
				handle_state_commands(command);
				break;

			case C_PLAYER_A:
				handle_player_command(command);
				break;
			
			case C_PLAYER_B:
				handle_player_command(command);
				break;




			case C_INVALID:
				printf("INVALID COMMAND\n");
				handle_invalid_command(command, activeCommandType);
				break;
		}	
	}

	fleet_free();
	//board_free(board, &dim); FIXME
	return 0;
}