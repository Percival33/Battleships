#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "constants.h"
#include "vector.h"
#include "save.h"
#include "board.h"
#include "ship.h"
#include "player.h"

void save_board_size(vector_t* v, dim_t* dim) {

    char command[MAX_COMMAND_LENGTH];
    sprintf(command, "%s %d %d", BOARD_SIZE_CHAR, dim->ROWS, dim->COLS);
    push_back(v, command);

    return;
}

void save_next_player(vector_t* v, int* nextPlayer) {

    char command[MAX_COMMAND_LENGTH];
    sprintf(command, "%s %c", NEXT_PLAYER_CHAR, (*nextPlayer == PLAYER_A ? 'A' : 'B'));
    push_back(v, command);

    return;
}

void save_init_position(vector_t* v, player_t* player) {

    char command[MAX_COMMAND_LENGTH];
    char P = (player->id == PLAYER_A ? 'A' : 'B');
    field_t leftUp;
    field_t rightDown;

    leftUp.x = player->colLow;
    leftUp.y = player->rowLow;

    rightDown.x = player->colHigh - 1;
    rightDown.y = player->rowHigh - 1;

    sprintf(command, "%s %c %d %d %d %d", INIT_POSITION_CHAR, P, leftUp.y, leftUp.x, rightDown.y, rightDown.x);

    push_back(v, command);
}

int get_max_shipId(player_t* player, int cls) {
    int res = -1;

    for (int shipId = 0; shipId < MAX_SHIPS_NUMBER; shipId++) {
        if (player->ships[cls][shipId].created == False)
            return res + 1;
        res = shipId;
    }
    assert(res == -1);
    return res + 1;
}

void save_size_of_fleet(vector_t* v, player_t* player) {

    char command[MAX_COMMAND_LENGTH];
    char P = (player->id == PLAYER_A ? 'A' : 'B');
    int fleet[MAX_SHIP_TYPE_NUMBER];

    for (int cls = S_DES; cls <= S_CAR; cls++) {
        fleet[cls] = get_max_shipId(player, cls);
    }

    sprintf(command, "%s %c %d %d %d %d", SET_FLEET_CHAR, P, fleet[S_CAR], fleet[S_BAT], fleet[S_CRU], fleet[S_DES]);
    push_back(v, command);
}

void reverse_get_status(char status[], int damaged[], int cls) {
    for (int len = 0; len < cls; len++) {
        status[len] = (damaged[len] == True ? '0' : '1');
    }
    return;
}

char* save_ship_command(player_t* player, int cls, int shipId) {
    // SHIP P y x D i C a1...al
    char command[MAX_COMMAND_LENGTH];
    char P = (player->id == PLAYER_A ? 'A' : 'B');

    ship_t ship = player->ships[cls][shipId];

    char dir = reverse_get_dir(ship.direction);
    char* clsChar = reverse_get_cls(cls);
    char statusChar[MAX_SHIP_TYPE_NUMBER];
    reverse_get_status(&statusChar, ship.damaged, cls);
    statusChar[cls] = '\0';

    sprintf(command, "%s %c %d %d %c %d %s %s", SHIP_CHAR, P, ship.head.y, ship.head.x, dir, shipId, clsChar, statusChar);

    char* strr;
    strr = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));

    strcpy(strr, command);

    return strr;
}

void save_all_ships_of_player(vector_t* v, player_t* player) {

    for (int cls = S_CAR; cls >= S_DES; cls--) {

        for (int shipId = 0; shipId < player->fleet[cls]; shipId++) {
            if (player->ships[cls][shipId].placed == True) {
                char command[MAX_COMMAND_LENGTH];
                char* str;
                str = save_ship_command(player, cls, shipId);
                strcpy(command, str);
                push_back(v, command);

                free(str);
            }
        }
    }

}
void save_reefs(vector_t* v, vector_t* reefs) {

    if (reefs->count == 0) {
        assert(reefs->allocatedSize == 1);
        return;
    }

    for (int i = 0; i < reefs->count; i++) {
        char text[MAX_COMMAND_LENGTH];
        push_back(v, strcpy(text, reefs->ptr[i].text));
    }

    return;
}

void save_extended_ships(vector_t* v, int extendedShips) {

    if (extendedShips == True) {
        char command[MAX_COMMAND_LENGTH];
        sprintf(command, "%s", "EXTENDED_SHIPS");
        push_back(v, command);
    }

    return;
}

void save_given_seed(vector_t* v, int* seed) {
    char command[MAX_COMMAND_LENGTH];

    sprintf(command, "%s %d", "SRAND", *seed + 1);
    push_back(v, command);

    return;
}

void save_players_game_state(vector_t* v, player_t** players, int playerId) {
    save_init_position(v, players[playerId]);
    save_size_of_fleet(v, players[playerId]);
    save_all_ships_of_player(v, players[playerId]);

    return;
}

void save_game_state(vector_t* v, vector_t* reefs, board_t** board, dim_t* dim,
    player_t** players, int* nextPlayer, int extendedShips, int* seed, int aiPlayer) {

    free(v->ptr);
    init(v);

    save_board_size(v, dim);
    save_next_player(v, nextPlayer);

    save_players_game_state(v, players, PLAYER_A);
    save_players_game_state(v, players, PLAYER_B);

    save_reefs(v, reefs);
    save_extended_ships(v, extendedShips);

    if (*seed != DEFAULT_SEED)
        save_given_seed(v, seed);

    printf("%s\n", STATE_CHAR);
    print_self(v);
    printf("%s\n", STATE_CHAR);
}