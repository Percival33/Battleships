#pragma once
#include "constants.h"

typedef struct dimensions {
	int ROWS;
	int COLS;
}dim_t;

typedef struct board {
	int type;
	int visible;
	int visited;
	int spy;
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
	Function prints board in STATE command
*/
void state_board_print(board_t** board, dim_t* dim, int mode, struct player_t** players);

/*
	Function prints board in PLAYER command
*/
void player_board_print(board_t** board, dim_t* dim, struct player_t** players, int playerId, int mode, int extendedShips);

bool check_ship_fits_on_board(field_t field, int dir, int cls, struct player_t* player, board_t** board, dim_t* dim);

bool check_around(board_t** board, field_t field, dim_t* dim);

bool check_neighbouring_fields(board_t** board, field_t field, dim_t* dim, int cls, int dir);

bool check_if_free_from_reef(board_t** board, dim_t* dim, field_t field, int cls, int dir);

bool check_field_on_board(dim_t* dim, field_t field);

void set_reef(char command[], board_t** board, dim_t* dim);

dim_t set_dim_size(char command[]);

void set_board_size(char command[], board_t** board, dim_t* dim);

void set_init_position(char command[], struct player_t** players, dim_t* dim);

bool check_coords_inside_player_area(field_t field, int dir, int cls, struct player_t* player);

void clear_visited(board_t** board, dim_t* dim);

bool field_occupied_by_ship(board_t** board, field_t target);