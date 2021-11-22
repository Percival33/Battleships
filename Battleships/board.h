#pragma once

typedef struct {
	int type;
}board_t;

board_t** board_init(const int ROWS, const int COLS);

void board_print(board_t** board, const int ROWS, const int COLS);

void board_free(board_t** board, const int ROWS);