#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "player.h"
#include "board.h"

dim_t* dim_init(int ROWS, int COLS) {
	dim_t* dim = (dim_t*)malloc(sizeof(dim));

	assert(dim != NULL);

	dim->COLS = 10;
	dim->ROWS = 10;

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

int board_inside(dim_t* dim, field_t* field) {
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	int inRows = (0 <= field->y && field->y < ROWS && field->y != ROWS/2);
	int inCols = (0 <= field->x && field->x < COLS);

	if (inRows && inCols) {
		return True;
	}
	return False;
}