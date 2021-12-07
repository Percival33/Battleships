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
			board[i][j].spy = B_EMPTY;
			board[i][j].playerId = B_EMPTY;
			board[i][j].cls = B_EMPTY;
			board[i][j].shipId = B_EMPTY;
		}
	}

	return board;
}

bool check_ship_fits_on_board(field_t field, int dir, int cls, player_t* player, board_t** board,
	dim_t* dim) {

	for (int len = 0; len < cls; len++) {

		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		if (check_field_on_board(dim, field) == False)
			return False;
	}
	return True;
}

bool check_around(board_t** board, field_t field, dim_t* dim) {
	for (int dir = 0; dir < 4; dir++) {
		int newX = field.x + dx[dir];
		int newY = field.y + dy[dir];

		if (newX >= dim->COLS || newY >= dim->ROWS || newX < 0 || newY < 0) // field does not exist
			continue;

		int fieldType = board[newY][newX].type;

		if (B_TAKEN <= fieldType && fieldType < B_DESTROYED) { // check if fieldType is occupied by ship
			return False;
		}
	}
	return True;
}

bool check_neighbouring_fields(board_t** board, field_t field, dim_t* dim, int cls, int dir) {
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}
		if (!check_around(board, field, dim)) {
			return False;
		}
	}
	return True;
}

bool check_if_free_from_reef(board_t** board, field_t field, int cls, int dir) {
	for (int len = 0; len < cls; len++) {

		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		if (board[field.y][field.x].type == B_REEF)
			return False;
	}
	return True;
}

bool check_field_on_board(dim_t* dim, field_t field) {
	const int ROWS = dim->ROWS;
	const int COLS = dim->COLS;

	int inRows = (0 <= field.y && field.y < ROWS);
	int inCols = (0 <= field.x && field.x < COLS);

	if (inRows && inCols)
		return True;
	return False;
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
				int spy = board[row][col].spy;
				int playerId = player->id;

				if (spy == playerId || spy == B_VISIBLE_BOTH || 
					visible == playerId || visible == B_VISIBLE_BOTH) { // field is visible by spy or radar
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
		return ' ';
	case B_TAKEN:
		return '+';
	case B_ENGINE:
		return '%';
	case B_CANNON:
		return '!';
	case B_RADAR:
		return '@';
	case B_DESTROYED:
		return 'x';
	case B_REEF:
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
			if (mode == PLAYER_A) { // print from player's perspective
				int visible = board[row][col].visible;
				int spy = board[row][col].spy;
				int playerId = player->id;

				if (spy == playerId || spy == B_VISIBLE_BOTH ||
					visible == playerId || visible == B_VISIBLE_BOTH) { // field is visible by spy or radar
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

void set_visible(board_t** board, dim_t* dim, field_t target, field_t origin, int range, int playerId) {
	if (check_field_on_board(dim, target) == False)
		return;
	if (board[target.y][target.x].visited == True)
		return;
	if (get_dist(target, origin) > range)
		return;

	board[target.y][target.x].visited = True;

	int visible = board[target.y][target.x].visible;

	// if ship continue ?

	if (visible == B_EMPTY) { // default value -> seen for the first time
		visible = playerId;
	}
	else if (visible != playerId) {
		assert(visible != B_EMPTY);
		assert(visible == playerId ^ 1);
		visible = B_VISIBLE_BOTH;
	}

	assert(visible != B_EMPTY);
	board[target.y][target.x].visible = visible;

	for (int dir = N; dir <= NW; dir++) {
		field_t newTarget;
		newTarget.x = target.x + dx[dir];
		newTarget.y = target.y + dy[dir];

		set_visible(board, dim, newTarget, origin, range, playerId);
	}
	assert(board[target.y][target.x].visible != B_EMPTY);
	return;
}

void clear_visited(board_t** board, dim_t* dim) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;


	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = COL_LOW; col < COL_HIGH; col++) {
			board[row][col].visited = B_EMPTY;
		}
	}

	return;
}

void add_visible_fields_from_ship(board_t** board, dim_t* dim, player_t* player, int cls, int shipId) {
	field_t head = player->ships[cls][shipId].head;
	int dir = player->ships[cls][shipId].direction;
	
	if (player->ships[cls][shipId].damaged[0] == True) // radar is broken
		set_visible(board, dim, head, head, 1, player->id); // origin = field, range = 1 
	else
		set_visible(board, dim, head, head, cls * cls, player->id); // origin = field, range = class squared
	
	clear_visited(board, dim);
	
	for (int len = 0; len < cls; len++) {
		if (len != 0) {
			head.x += dx[dir];
			head.y += dy[dir];
		}
		
		set_visible(board, dim, head, head, 1, player->id); // mark ship position
	}
	return;
}

void clear_visible(board_t** board, dim_t* dim) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;

	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = COL_LOW; col < COL_HIGH; col++) {
			board[row][col].visible = B_EMPTY;
		}
	}
}

void add_visible_fields_on_board(board_t** board, dim_t* dim, player_t* player, int extendedShips) {

	clear_visible(board, dim);

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
			if (player->ships[cls][shipId].created == False) {
				continue;
			}
			add_visible_fields_from_ship(board, dim, player, cls, shipId);
		}
	}

	clear_visited(board, dim);

	return;
}

void player_board_print(board_t** board, dim_t* dim, player_t** players, int playerId, int mode, int extendedShips) {

	add_visible_fields_on_board(board, dim, players[playerId], extendedShips);

	if (mode == 0) {
		basic_print(board, dim, players[playerId], PLAYER_A); // player printing
	}
	else if (mode == 1) {
		extended_print(board, dim, players[playerId], PLAYER_A); // player printing
	}

	return;

}

void set_reef(char command[], board_t** board, dim_t* dim) {
	int y;
	int x;
	int argc = sscanf(command, "%*s %d %d", &y, &x);
	if (argc != 2) {
		handle_invalid_command(command, C_INVALID);
	}

	if (y < 0 || y >= dim->ROWS || x < 0 || x >= dim->COLS) {
		handle_invalid_command(command, C_REEF_NOT_ON_BOARD);
	}

	board[y][x].type = B_REEF;
	return;

}

dim_t set_dim_size(char command[]) {
	int y;
	int x;
	int argc = sscanf(command, "%*s %d %d", &y, &x);

	if (argc != 2 || y <= 0 || x <= 0) {
		handle_invalid_command(command, C_INVALID);
	}

	dim_t dim;
	dim.ROWS = y;
	dim.COLS = x;
	return dim;
}

void set_board_size(char command[], board_t** board, dim_t* dim) {
	if (dim->ROWS != DEFAULT_COLS_NUMBER && dim->COLS != DEFAULT_COLS_NUMBER) {
		handle_invalid_command(command, C_INVALID);
	}
	board_free(board, dim);
	dim_t dim_local = set_dim_size(command);
	dim->ROWS = dim_local.ROWS;
	dim->COLS = dim_local.COLS;
	board = board_init(dim);
	return;
}

void set_init_position(char command[], player_t** players, dim_t* dim) {
	//INIT_POSITION P y1 x1 y2 x2 
	int yL;
	int yR;
	int xL;
	int xR;
	char P;

	int argc = sscanf(command, "%*s %c %d %d %d %d", &P, &yL, &xL, &yR, &xR);

	int playerId = get_player_id(P);

	if (argc != 5) {
		handle_invalid_command(command, C_INVALID);
	}
	if (playerId == ERROR) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}
	if ((yL < 0 || yL > dim->ROWS) ||
		(xL < 0 || xL > dim->COLS) ||
		(xR < xL || yR < yL) ||
		(xR > dim->COLS || yR > dim->ROWS)) {
		handle_invalid_command(command, C_WRONG_ARGS);
	}

	players[playerId]->rowLow = yL;
	players[playerId]->rowHigh = yR + 1;
	players[playerId]->colLow = xL;
	players[playerId]->colHigh = xR + 1;


	return;
}

bool check_coords_inside_player_area(field_t field, int dir, int cls, player_t* player) {

	int inRows;
	int inCols;

	for (int len = 0; len < cls; len++) {

		if (len != 0) {
			field.y += dy[dir];
			field.x += dx[dir];
		}

		inRows = (player->rowLow <= field.y && field.y < player->rowHigh);
		inCols = (player->colLow <= field.x && field.x < player->colHigh);

		if (!inRows || !inCols)
			return False;

	}
	return True;
}


