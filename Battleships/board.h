#pragma once

typedef struct dimensions {
	int ROWS;
	int COLS;
}dim_t;

typedef struct board {
	int type;
	int playerId;
	int cls;
	int shipId;
}board_t;


/*
	Initialize structure with dimentions
*/
dim_t* dim_init(int ROWS, int COLS);

/*
	Initialize board of dim dimentions
*/
board_t** board_init(dim_t* dim);

/*
	free memory allocated for board and each row of it
*/
void board_free(board_t** board, dim_t* dim);

/*
	Function prints board
*/
void board_print(board_t** board, dim_t* dim, int mode, struct player_t** players);