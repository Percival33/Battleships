#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "player.h"
#include "board.h"

dim_t* dim_init(int ROWS, int COLS) {
	dim_t* dim = (dim_t*)malloc(sizeof(dim));

	assert(dim != NULL);

	dim->COLS = COLS;
	dim->ROWS = ROWS;

	return dim;
}

board_t** board_init(dim_t* dim) {
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	board_t** board = (board_t**)malloc(ROWS * sizeof(board_t*));

	assert(board != NULL);

	for (int i = 0; i < ROWS; i++) {
		board[i] = (board_t*)malloc(COLS * sizeof(board_t));
		assert(board[i] != NULL);
	}


	for (int i = 0; i < COLS; i++) {
		board[ROWS / 2][i].type = B_BAN;
	}

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			board[i][j].type = B_EMPTY;
			board[i][j].visible = B_EMPTY;
			board[i][j].visited = B_EMPTY;
			board[i][j].visTimes = 0;
			board[i][j].playerId = B_EMPTY;
			board[i][j].cls = B_EMPTY;
			board[i][j].shipId = B_EMPTY;
		}
	}

	return board;
}

void board_free(board_t** board, dim_t* dim) {
	for (int i = 0; i < dim->ROWS; i++)
		free(board[i]);
	free(board);
	return;
}

char basic_char_field(board_t** board, int row, int col) {
	if (board[row][col].type == B_TAKEN ||
		board[row][col].type == B_ENGINE ||
		board[row][col].type == B_CANNON ||
		board[row][col].type == B_RADAR) {
		//printf("+");
		return '+';
	}
	else if (board[row][col].type == B_DESTROYED) {
		//printf("x");
		return 'x';
	}
	else {
		//printf(" ");
		return ' ';
	}
	return '=';
}

void basic_print(board_t** board, dim_t* dim, player_t* player, int mode) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;

	assert(mode == PLAYER_A || mode == C_STATE_TYPE); // only two allowed values of mode variable

	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = COL_LOW; col < COL_HIGH; col++) {
			if (mode == PLAYER_A) { // print from player's perspective
				int visible = board[row][col].visible;
				int playerId = player->id;

				if (visible == playerId || visible == B_VISIBLE_BOTH) { // field is not visible at all or not visibile for this player
					printf("%c", basic_char_field(board, row, col));
				}
				else {
					printf("?");
				}
			}
			else {
				printf("%c", basic_char_field(board, row, col));
			}
			//printf("{%d,%d} = %d ", i, j, board[i][j].type);
		}
		printf("\n");
	}

	return;
}

void extended_print_num_cols_rows(int COLS) {
	if (COLS > 10) {
		printf("  ");
		for (int num = 0; num < COLS; num++) {
			printf("%d", num / 10);
		}
		printf("\n");
		printf("  ");
		for (int num = 0; num < COLS; num++) {
			printf("%d", num % 10);
		}
		printf("\n");
	}
	else {
		printf("  ");
		for (int num = 0; num < COLS; num++) {
			printf("%d", num);
		}
		printf("\n");
	}
	return;
}

char extended_char_field(board_t** board, int row, int col) {
	switch (board[row][col].type) {
	case B_EMPTY:
		//printf(" ");
		return ' ';
	case B_TAKEN:
		//printf("+");
		return '+';
	case B_ENGINE:
		//printf("%%");
		return '%';
	case B_CANNON:
		//printf("!");
		return '!';
	case B_RADAR:
		//printf("@");
		return '@';
	case B_DESTROYED:
		//printf("x");
		return 'x';
	case B_REEF:
		//printf("#");
		return '#';
	}
	return '=';
}

void extended_print(board_t** board, dim_t* dim, player_t* player, int mode) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;

	

	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		if (row == ROW_LOW) {
			extended_print_num_cols_rows(COL_HIGH);
		}
		
		printf("%02d", row);

		for (int col = COL_LOW; col < COL_HIGH; col++) {
			if (mode == PLAYER_A) {
				if (board[row][col].visible == board[row][col].playerId ||
					board[row][col].visible == B_VISIBLE_BOTH) {			// field visible for a player
					printf("%c", extended_char_field(board, row, col));
				}
				else {
					printf("?");
				}
			}
			else {
				printf("%c", extended_char_field(board, row, col));
			}
		}
		printf("\n");
	}

	return;
}

void state_board_print(board_t** board, dim_t* dim, int mode, player_t** players) {
		
	if (mode == 0) {
		basic_print(board, dim, players[PLAYER_A], C_STATE_TYPE); // state printing
	}
	else if (mode == 1) {
		extended_print(board, dim, players[PLAYER_A], C_STATE_TYPE); // state printing
	}

	int AReamaining = get_remaining_parts(players[PLAYER_A]);
	int BReamaining = get_remaining_parts(players[PLAYER_B]);


	printf("PARTS REMAINING:: A : %d B : %d\n", AReamaining, BReamaining);

	return;

}

//TODO add to handle command function
void player_board_print(board_t** board, dim_t* dim, player_t** players, int playerId, int mode) {

	if (mode == 0) {
		basic_print(board, dim, players[playerId], PLAYER_A); // player printing
	}
	else if (mode == 1) {
		extended_print(board, dim, players[playerId], PLAYER_A); // player printing
	}

	return;

}