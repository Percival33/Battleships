#pragma once

typedef struct board {
	int rows;
	int cols;
	int type;
}board_t;

board_t** board_init(const int ROWS, const int COLS);

void board_print(board_t** board, const int ROWS, const int COLS);

void board_free(board_t** board, const int ROWS);