#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vector.h"
#include "save.h"
#include "player.h"
#include "board.h"
#include "ship.h"
#include "commands.h"
#include "board_ship_func.h"

#define MAX_TRIES 10*1000

int rand_range(int a, int b) {
	if (a > b)
		return ERROR;

	return rand() % (b - a + 1) + a;
}

bool validate_place_ship(board_t** board, dim_t* dim, player_t* player, field_t head, int dir, int cls) {
	if (check_ship_fits_on_board(head, dir, cls, player, board, dim) == False) {
		return False;
	}
	if (check_coords_inside_player_area(head, dir, cls, player) == False) {
		return False;
	}
	if (check_if_free_from_reef(board, head, cls, dir) == False) {
		return False;
	}
	if (!check_neighbouring_fields(board, head, dim, cls, dir)) {
		return False;
	}

	return True;
}

void validate_tries(int rep) {
	if (rep == MAX_TRIES) {
		handle_invalid_command("rand goes brr", C_TOO_MANY_RAND);
	}
	return;
}

void ai_place_one_ship(vector_t* ai, board_t** board, dim_t* dim, player_t* player, int cls, int shipId) {
	field_t head;
	int dir;
	int foundPlace = 0;
	int rep = 0;

	while (!foundPlace) {
		head.x = rand_range(player->colLow, player->colHigh);
		head.y = rand_range(player->rowLow, player->rowHigh);

		dir = rand_range(N, W);

		if (validate_place_ship(board, dim, player, head, dir, cls) == True) {
			foundPlace = True;
			break;
		}

		validate_tries(rep);

		rep++;
	}

	char command[MAX_COMMAND_LENGTH];
	char* clsChar;
	char dirChar;

	clsChar = reverse_get_cls(cls);
	dirChar = reverse_get_dir(dir);

	int temp = sprintf(command, "%s %d %d %c %d %s", PLACE_SHIP_CHAR, head.y, head.x, dirChar, shipId, clsChar);

	push_back(ai, command);

	return;
}

void ai_place_all_ships(vector_t* ai, board_t** board, dim_t* dim, player_t* player) {
	for (int cls = S_CAR; cls >= S_DES; cls--) {
		for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
			ai_place_one_ship(ai, board, dim, player, cls, shipId);
		}
	}
}

bool validate_move_command(board_t** board, dim_t* dim, player_t* player, int cls, int shipId, int extendedShips, int moveDir) {
	if (shipId == ERROR || shipId < 0 || shipId > player->fleet[cls]) {
		return False;
	}
	if (player->ships[cls][shipId].moved == get_number_of_moves(cls)) {
		return False;
	}
	if (extendedShips == True && player->ships[cls][shipId].damaged[cls - 1] == True) { // engine destroyed
		return False;
	}

	int flag = check_if_ship_can_go(board, player, dim, cls, shipId, moveDir);

	if (flag != True) {
		return False;
	}

	return True;
}

void ai_move_one_ship(vector_t* ai, board_t** board, dim_t* dim, player_t* player, int extendedShips, int cls, int shipId) {
	//int cls;
	//int shipId;
	int moveDir;
	int rep = 0;
	int foundPlace = 0;

	while (!foundPlace) {
		
		moveDir = rand_range(L, R);							// [L, F, R]

		if (validate_move_command(board, dim, player, cls, shipId, extendedShips, moveDir) == True) {
			foundPlace = True;
			break;
		}

		validate_tries(rep);

		rep++;
	}

	char command[MAX_COMMAND_LENGTH];
	char* clsChar;
	char moveDirChar;

	clsChar = reverse_get_cls(cls);
	moveDirChar = reverse_get_dir_move(moveDir);

	int temp = sprintf(command, "%s %d %s %c", MOVE_CHAR, shipId, clsChar, moveDirChar);

	push_back(ai, command);
	return;
}

bool extended_validate_searching_enemy(board_t** board, dim_t* dim, field_t target,
	field_t radar, field_t cannon, int cls, int radarRange) {

	if (check_field_on_board(dim, target) == False)
		return False;
	if (board[target.y][target.x].visited == True)
		return False;
	if (get_dist(target, radar) > radarRange)
		return False;
	if (get_dist(target, cannon) > get_shooting_range(cls))
		return False;

	return True;
}

field_t extended_ai_search_enemy_from_ship(board_t** board, dim_t* dim, field_t target, 
	field_t radar, field_t cannon, int cls, int radarRange, int aiPlayer) 
{
	field_t empty;
	empty.y = -1;
	empty.x = -1;

	if (extended_validate_searching_enemy(board, dim, target, radar, cannon, cls, radarRange) == False) {
		return empty;
	}

	board[target.y][target.x].visited = True;

	if (board[target.y][target.x].type != B_DESTROYED && board[target.y][target.x].shipId == aiPlayer ^ 1) { // different player
		return target;
	}

	for (int dir = N; dir <= NW; dir++) {
		field_t newTarget = target;
		newTarget.y += dy[dir];
		newTarget.x += dx[dir];

		field_t enemy = extended_ai_search_enemy_from_ship(board, dim, newTarget, radar, cannon,
			cls, radarRange, aiPlayer);

		if (enemy.y != -1 && enemy.x != -1) // enemy found within range
			return enemy;
	}

	return empty;
}


bool basic_validate_searching_enemy(board_t** board, dim_t* dim, field_t target, 
	field_t cannon, int cls) {
	if (check_field_on_board(dim, target) == False)
		return False;
	if (board[target.y][target.x].visited == True)
		return False;
	if (get_dist(target, cannon) > get_shooting_range(cls))
		return False;
}

bool seen_by_spy(board_t** board, field_t target, int playerId) {
	return (board[target.y][target.x].spy == B_VISIBLE_BOTH ||
		board[target.y][target.x].spy == playerId);
}

field_t basic_ai_search_enemy_from_ship(board_t** board, dim_t* dim, field_t target, field_t cannon, int cls, int aiPlayer) {
	field_t empty;
	empty.y = -1;
	empty.x = -1;

	if (basic_validate_searching_enemy(board, dim, target, cannon, cls) == False) {
		return empty;
	}

	board[target.y][target.x].visited = True;

	if (seen_by_spy(board, target, aiPlayer) && board[target.y][target.x].type != B_DESTROYED &&
		board[target.y][target.x].shipId == aiPlayer ^ 1) { // different player
		return target;
	}

	for (int dir = N; dir <= NW; dir++) {
		field_t newTarget = target;
		newTarget.y += dy[dir];
		newTarget.x += dx[dir];

		field_t enemy = basic_ai_search_enemy_from_ship(board, dim, newTarget, cannon,
			cls, aiPlayer);

		if (enemy.y != -1 && enemy.x != -1) // enemy found within range
			return enemy;
	}

	return empty;
}

void set_ship_fields(ship_t ship, field_t* radar, field_t* cannon) {
	radar->x = ship.head.x;
	radar->y = ship.head.y;

	cannon->x = ship.head.x +dx[ship.direction];
	cannon->y = ship.head.y +dy[ship.direction];

	return;
}

bool validate_shooting_abilities(player_t* player, int cls, int shipId, int* radarRange) {
	if (player->ships[cls][shipId].damaged[1] == True)	// cannon is damaged
		return False;

	*radarRange = cls * cls;							// radar range = cls, but squared 
	
	if (player->ships[cls][shipId].damaged[0] == True)	// radar is broken
		*radarRange = 1;

	return True;
}

field_t ai_search_enemy(board_t** board, dim_t* dim, player_t* player, int extendedShips, int aiPlayer) {
	// return field with enemy ship within shooting range
	field_t enemy;
	field_t radar;
	field_t cannon;

	enemy.x = -1;
	enemy.y = -1;

	int radarRange;

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
			clear_visited(board, dim);
			
			set_ship_fields(player->ships[cls][shipId], &radar, &cannon);

			if (extendedShips) {
				
				if (validate_shooting_abilities(player, cls, shipId, &radarRange) == False)
					continue;
				if (radarRange == 1) // can shoot at field seen by spy plane
					enemy = basic_ai_search_enemy_from_ship(board, dim, cannon, cannon, cls, aiPlayer);
				else
					enemy = extended_ai_search_enemy_from_ship(board, dim, radar, radar, cannon,
						cls, radarRange, aiPlayer, extendedShips);

				if (enemy.x != -1 && enemy.y != -1) // enemy found
					return enemy;
			}
			else {

				enemy = basic_ai_search_enemy_from_ship(board, dim, cannon, cannon, cls, aiPlayer);

				if (enemy.x != -1 && enemy.y != -1) // enemy found
					return enemy;

			}
			// if cannon is not destroyed
			// radarRange squared
			//ai_search_enemy_from_ship()
		}
	}
	assert(enemy.x == -1 && enemy.y == -1);
	return enemy; // no enemy found
}

void ai_move_random_ship(vector_t* ai, board_t** board, dim_t* dim, player_t* player, int extendedShips) {
	int cls = rand_range(S_DES, S_CAR);
	int shipId = rand_range(0, player->fleet[cls] - 1);		//shipId from 0 to fleet[cls] - 1
	ai_move_one_ship(ai, board, dim, player, extendedShips, cls, shipId);
}

void ai_shoot_from_ship(vector_t* v, field_t target, int shipId, int cls) {
	char command[MAX_COMMAND_LENGTH];

	char* clsChar = reverse_get_cls(cls);

	int temp = sprintf(command, "%s %d %s %d %d", SHOOT_CHAR, shipId, clsChar, target.y, target.x);

	push_back(v, command);
}

void ai_shoot(vector_t* v, board_t** board, dim_t* dim, player_t* player, field_t target, int extendedShips) {
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
			//if can shoot, save command, rand move ship?
			field_t cannon = player->ships[cls][shipId].head;
			cannon.x += dx[player->ships[cls][shipId].direction];
			cannon.y += dy[player->ships[cls][shipId].direction];
			
			if (get_dist(target, cannon) <= get_shooting_range(cls)) {
				ai_shoot_from_ship(v, target, shipId, cls);

				int r = rand_range(0, 100);

				if(r%15 == 0)
					ai_move_one_ship(v, board, dim, place_ship, extendedShips, cls, shipId);

				return;
			}
		}
	}
	assert(0 == -1); 
	return;
}

void run_ai(vector_t* v, vector_t* reefs, board_t** board, dim_t* dim,
	player_t** players, int extendedShips, int* seed, int aiPlayer) {
	
	save_geme_state(v, reefs, board, dim, players, aiPlayer, extendedShips, seed);
	
	printf("%s\n %s\n %s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);

	int cls;
	int shipId;
	int shots = 0;
	vector_t aiCommands;
	
	init(&aiCommands);

	ai_place_all_ships(&aiCommands, board, dim, players[aiPlayer]);

	field_t enemy = ai_search_enemy(board, dim, players[aiPlayer], extendedShips, aiPlayer);
	
	if (extendedShips) {
		while (enemy.x != -1 && enemy.y != -1) {
			// if can shoot, else break
			ai_shoot(&aiCommands, board, dim, players[aiPlayer], enemy, extendedShips);		// shoot at enemy
			enemy = ai_search_enemy(board, dim, players[aiPlayer], extendedShips, aiPlayer);
		}
	}
	else {
		ai_shoot(&aiCommands, board, dim, players[aiPlayer], enemy, extendedShips);
	}
	
	ai_move_random_ship(&aiCommands, board, dim, players[aiPlayer], extendedShips);
	
	print_self(&aiCommands);

	printf("%s\n %s\n %s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);

	return;
}