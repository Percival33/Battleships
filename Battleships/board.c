#include <stdlib.h>
#include <assert.h>

#include "constants.h"
#include "board.h"

board_t** board_init(const int ROWS, const int COLS) {
	board_t** board = (board_t**)malloc(ROWS * sizeof(board_t*));
	
	assert(board != NULL);

	for (int i = 0; i < ROWS; i++) {
		board[i] = (board_t*)malloc(COLS * sizeof(board_t));
		assert(board[i] != NULL);
	}


	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			board[i][j].type = B_EMPTY;
		}
	}

	for (int i = 0; i < COLS; i++) {
		board[10][i].type = B_TAKEN;	// assuring that middle row between boards of 
										// each players is not allowing to place any ship there
	}

	return board;
}

void board_print(board_t** board, const int ROWS, const int COLS) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			printf("{%d,%d} = %d ", i, j, board[i][j].type);
		}
		printf("\n");
	}
}

void board_free(board_t** board, const int ROWS) {
	for (int i = 0; i < ROWS; i++) {
		free(board[i]);
	}
	free(board);
	return;
}