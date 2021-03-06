#pragma once

void set_ship(char command[], struct board_t** board, struct player_t** players, struct dim_t* dim);

void place_ship(char command[], struct board_t** board, struct player_t* player, struct dim_t* dim);

void add_ship_on_board(struct board_t** board, struct dim_t* dim, field_t field, 
	struct player_t* player, int cls, int dir, int shipId);

void remove_ship_from_board(struct board_t** board, field_t field, struct player_t* player, int cls, int dir);