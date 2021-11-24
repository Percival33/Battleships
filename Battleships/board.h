#pragma once

typedef struct dimensions {
	int ROWS;
	int COLS;
}dim_t;

typedef struct board {
	int type;
}board_t;

board_t** board_init(dim_t* dim);

void board_print(board_t** board, dim_t* dim);


/*
	free memory allocated for board
*/
void board_free(board_t** board, dim_t* dim);

/*
	check if field is in position of a player
*/
int board_inside(dim_t* dim, struct field_t* field);