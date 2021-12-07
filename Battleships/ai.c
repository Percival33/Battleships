#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "vector.h"
#include "save.h"
#include "player.h"
#include "board.h"
#include "ship.h"
#include "commands.h"
#include "board_ship_func.h"

#define MAX_TRIES 1000*1000
#define TRIES 30

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
	if (check_neighbouring_fields(board, head, dim, cls, dir) == False) {
		return False;
	}

	return True;
}

void validate_tries(int rep) {
	if (rep == MAX_TRIES) {
		handle_invalid_command("rand goes brrr", C_TOO_MANY_RAND);
	}
	return;
}

void ai_place_one_ship(vector_t* ai, board_t** board, dim_t* dim, player_t* player, int cls, int shipId) {
	field_t head;
	int dir;
	bool foundPlace = False;
	int rep = 0;

	while (foundPlace == False) {
		

		dir = rand_range(N, W);

		for (int i = 0; i < TRIES; i++) {

			head.x = rand_range(player->colLow, player->colHigh - 1);
			head.y = rand_range(player->rowLow, player->rowHigh -1);
			
			if (validate_place_ship(board, dim, player, head, dir, cls) == True) {
				foundPlace = True;
				break;
			}		
		
		}

		validate_tries(rep);

		rep++;
	}

	char command[MAX_COMMAND_LENGTH];
	char* clsChar;
	char dirChar;

	clsChar = reverse_get_cls(cls);
	dirChar = reverse_get_dir(dir);

	int temp = sprintf(command, "%s %d %d %c %d %s ", PLACE_SHIP_CHAR, head.y, head.x, dirChar, shipId, clsChar);

	assert(player->isAi == True);
	place_ship(command, board, player, dim);

	push_back(ai, command);

	return;
}

void ai_place_all_ships(vector_t* ai, board_t** board, dim_t* dim, player_t* player) {
	for (int cls = S_CAR; cls >= S_DES; cls--) {
		for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
			if(player->ships[cls][shipId].placed == False)
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
	bool foundPlace = False;

	while (foundPlace == False) {
		
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

	if (board[target.y][target.x].type != B_DESTROYED && board[target.y][target.x].playerId != aiPlayer &&
		board[target.y][target.x].type >= B_TAKEN && board[target.y][target.x].type <= B_RADAR) { // different player
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

bool seen_by_spy(board_t** board, field_t target, int playerId) {
	return (board[target.y][target.x].spy == B_VISIBLE_BOTH ||
		board[target.y][target.x].spy == playerId);
}

field_t basic_ai_search_enemy(board_t** board, dim_t* dim, int aiPlayer) {
	field_t empty;
	empty.y = -1;
	empty.x = -1;

	int ROWS = dim->ROWS;
	int COLS = dim->COLS;

	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			if (board[row][col].shipId != aiPlayer && 
				board[row][col].type >= B_TAKEN && board[row][col].type <= B_RADAR) { // field occupied by enemy's ship
				empty.x = col;
				empty.y = row;
				return empty;
			}
		}
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

	if (player->ships[cls][shipId].shots == cls)		// cannot shoot more
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
	if(extendedShips) {
		for (int cls = S_DES; cls <= S_CAR; cls++) {
			for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
				clear_visited(board, dim);
			
				set_ship_fields(player->ships[cls][shipId], &radar, &cannon);

				//if (extendedShips) {
				
					if (validate_shooting_abilities(player, cls, shipId, &radarRange) == False)
						continue;
					if (radarRange == 1) // can shoot at field seen by spy plane
						enemy = basic_ai_search_enemy(board, dim, aiPlayer);
					else
						enemy = extended_ai_search_enemy_from_ship(board, dim, radar, radar, cannon,
							cls, radarRange, aiPlayer);

					if (enemy.x != -1 && enemy.y != -1) // enemy found
						return enemy;
			}
		}
	}
	else {
		return basic_ai_search_enemy(board, dim, aiPlayer);
	}

	assert(enemy.x == -1 && enemy.y == -1);
	return enemy; // no enemy found
}

void ai_move_random_ship(vector_t* ai, board_t** board, dim_t* dim, player_t* player, int extendedShips) {
	bool found = False;
	int rep = 0;

	while (found == False) {
		int cls = rand_range(S_DES, S_CAR);
		
		if (player->fleet[cls] != 0) {
			int shipId = rand_range(0, player->fleet[cls] - 1);		//shipId from 0 to fleet[cls] - 1
			found = True;
			ai_move_one_ship(ai, board, dim, player, extendedShips, cls, shipId);
		}
	
		validate_tries(rep);
		rep++;
	}
	return;
}

void extended_ai_shoot_from_ship(vector_t* v, board_t** board, dim_t* dim, player_t** players,
	int aiPlayer, field_t target, int shipId, int cls)
{
	char command[MAX_COMMAND_LENGTH];

	char* clsChar = reverse_get_cls(cls);

	int temp = sprintf(command, "%s %d %s %d %d", SHOOT_CHAR, shipId, clsChar, target.y, target.x);

	shoot_extended(command, board, dim, players, aiPlayer); // shot on newBoard, to make sure, same field is not shot twice

	push_back(v, command);
}

void basic_ai_shoot(vector_t* v, board_t** board, dim_t* dim, player_t** players, field_t target, int aiPlayer) {
	char command[MAX_COMMAND_LENGTH];
	
	sprintf(command, "%s %d %d", SHOOT_CHAR, target.y, target.x);

	shoot_default(command, board, dim, players, aiPlayer);

	push_back(v, command);
	return;
}

void extended_ai_shoot(vector_t* v, board_t** board, dim_t* dim, player_t** players, int aiPlayer, field_t target, int extendedShips) {
	
	player_t* AI = players[aiPlayer];
	
	for (int cls = S_DES; cls <= S_CAR; cls++) {
		for (int shipId = 0; shipId < AI->fleet[cls]; shipId++) {
			field_t cannon = AI->ships[cls][shipId].head;
			cannon.x += dx[AI->ships[cls][shipId].direction];
			cannon.y += dy[AI->ships[cls][shipId].direction];

			int temp = ERROR;

			if (get_dist(target, cannon) <= get_shooting_range(cls) &&
				validate_shooting_abilities(AI, cls, shipId, &temp) == True) {
				if (extendedShips)
					extended_ai_shoot_from_ship(v, board, dim, players, aiPlayer, target, shipId, cls);
				else
					basic_ai_shoot(v, board, dim, players, target, aiPlayer);
				return;
			}
		}
	}
}

void ai_shoot(vector_t* v, board_t** board, dim_t* dim, player_t** players, int aiPlayer, field_t target, int extendedShips) {
	extended_ai_shoot(v, board, dim, players, aiPlayer, target, extendedShips);
	return;
}

void copy_player(player_t* dst, player_t* src) {
	dst->colHigh = src->colHigh;
	dst->colLow = src->colLow;
	dst->id = src->id;
	dst->isAi = src->isAi;
	dst->rowHigh = src->rowHigh;
	dst->rowLow = src->rowLow;
	dst->shipPlaced = src->shipPlaced;

	for (int cls = S_DES; cls <= S_CAR; cls++) {
		dst->fleet[cls] = src->fleet[cls];

		for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
			dst->ships[cls][shipId] = src->ships[cls][shipId];
		}
	}
	return;
}


void copy_board(board_t** dst, board_t** src, dim_t* dim) {
	const int COL_LOW = 0;
	const int COL_HIGH = dim->COLS;
	const int ROW_LOW = 0;
	const int ROW_HIGH = dim->ROWS;


	for (int row = ROW_LOW; row < ROW_HIGH; row++) {
		for (int col = COL_LOW; col < COL_HIGH; col++) {
			dst[row][col].type		=	src[row][col].type;
			dst[row][col].visible	=	src[row][col].visible;
			dst[row][col].visited	=	src[row][col].visited;
			dst[row][col].spy		=	src[row][col].spy;
			dst[row][col].playerId	=	src[row][col].playerId;
			dst[row][col].cls		=	src[row][col].cls;
			dst[row][col].shipId	=	src[row][col].shipId;
		}
	}

	return;
}

void print_ai(vector_t* aiCommands, int* aiPlayer) {
	printf("\n%s\n%s\n%s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);

	if (*aiPlayer == PLAYER_A)
		printf("\n%s\n", PLAYER_A_CHAR);
	else
		printf("\n%s\n", PLAYER_B_CHAR);

	print_self(aiCommands);

	if (*aiPlayer == PLAYER_A)
		printf("%s\n", PLAYER_A_CHAR);
	else
		printf("%s\n", PLAYER_B_CHAR);

	printf("\n%s\n%s\n%s\n", STATE_CHAR, "PRINT 0", STATE_CHAR);

	return;
}

void copy_data_for_ai(board_t** newBoard, board_t** board, dim_t* dim, player_t** players,
	player_t** newPlayer, int* aiPlayer) 
{
	copy_board(newBoard, board, dim);
	assert(newPlayer != NULL);

	newPlayer[PLAYER_A] = player_init(0, 0, PLAYER_A);
	newPlayer[PLAYER_B] = player_init(0, 0, PLAYER_B);

	copy_player(newPlayer[PLAYER_A], players[PLAYER_A]);
	copy_player(newPlayer[PLAYER_B], players[PLAYER_B]);

	//copy_player(newPlayer[*aiPlayer], players[*aiPlayer]);

	newPlayer[*aiPlayer]->isAi = True;
}

void run_ai(vector_t* v, vector_t* reefs, board_t** board, dim_t* dim,
	player_t** players, int* extendedShips, int* seed, int nextPlayer, int* aiPlayer) {


	board_t** newBoard = board_init(dim);
	player_t** newPlayer = (player_t**)malloc(2 * sizeof(player_t*));
	vector_t aiCommands;

	init(&aiCommands);

	copy_data_for_ai(newBoard, board, dim, players, newPlayer, aiPlayer);

	save_game_state(v, reefs, board, dim, players, nextPlayer, *extendedShips, seed, *aiPlayer);


	ai_place_all_ships(&aiCommands, newBoard, dim, newPlayer[*aiPlayer]);

	field_t enemy = ai_search_enemy(newBoard, dim, newPlayer[*aiPlayer], *extendedShips, *aiPlayer);

	if (*extendedShips) {
		if (enemy.x == -1 && enemy.y == -1) {

			for (int i = 0; i < TRIES; i++) {
				ai_move_random_ship(&aiCommands, newBoard, dim, newPlayer[*aiPlayer], *extendedShips);
				enemy = ai_search_enemy(newBoard, dim, newPlayer[*aiPlayer], *extendedShips, *aiPlayer);
				break;
			}
		}
		while (enemy.x != -1 && enemy.y != -1) {
			ai_shoot(&aiCommands, newBoard, dim, newPlayer, *aiPlayer, enemy, *extendedShips);		// shoot at enemy
			enemy = ai_search_enemy(newBoard, dim, newPlayer[*aiPlayer], *extendedShips, *aiPlayer);
		}
	}
	else { // shoot default -> infinite range -> always able to shoot
		ai_shoot(&aiCommands, newBoard, dim, newPlayer, *aiPlayer, enemy, *extendedShips);
	}

	
	ai_move_random_ship(&aiCommands, newBoard, dim, newPlayer[*aiPlayer], *extendedShips);
	
	print_ai(&aiCommands, aiPlayer);

	return;
}