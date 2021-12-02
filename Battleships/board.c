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
			board[i][j].playerId = B_EMPTY;
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

void basic_print(board_t** board, dim_t* dim) {
	const int COLS = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;

	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = 0; col < COLS; col++) {
			if (board[row][col].type == B_TAKEN		||
				board[row][col].type == B_ENGINE	||
				board[row][col].type == B_CANNON	||
				board[row][col].type == B_RADAR) {
				printf("+");
			}
			else if (board[row][col].type == B_DESTROYED) {
				printf("x");
			}
			else {
				printf(" ");
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

void extended_print(board_t** board, dim_t* dim, player_t** player) {
	const int COLS = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;

	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		if (row == ROW_LOW) {
			extended_print_num_cols_rows(COLS);
		}
		
		printf("%02d", row);

		for (int col = 0; col < COLS; col++) {
			switch (board[row][col].type) {
				case B_EMPTY:
					printf(" ");
					break;
				case B_TAKEN:
					printf("+");
					break;
				case B_ENGINE:
					printf("%%");
					break;
				case B_CANNON:
					printf("!");
					break;
				case B_RADAR:
					printf("@");
					break;
				case B_DESTROYED:
					printf("x");
					break;
				case B_REEF:
					printf("#");
					break;
			}
		}
		printf("\n");
	}

	return;
}

void board_print(board_t** board, dim_t* dim, int mode, player_t** player) {
		
	if (mode == 0) {
		basic_print(board, dim);
	}
	else if (mode == 1) {
		extended_print(board, dim, player);
	}

	int AReamaining = get_remaining_parts(player[PLAYER_A]);
	int BReamaining = get_remaining_parts(player[PLAYER_B]);


	printf("PARTS REMAINING:: A : %d B : %d\n", AReamaining, BReamaining);

	return;

}