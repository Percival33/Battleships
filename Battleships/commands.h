#pragma once

/*
	function reads command from input and returns its ID
*/
int get_command_type(char command[], int activeCommandType);

/*
	function handles invalid command, calls print_invalid_command and exits program
*/
void handle_invalid_command(char command[], int errorType);

/*
	function which checks if command is spelled correctly,
	then calls adequate function to deal with specific command
*/
void handle_state_commands(char command[], int* nextPlayer, struct board_t** board,
	struct player_t** players, struct dim_t* dim, int* extendedShips, struct vector_t* v,
	struct vector_t* reefs, int* seed, int* aiPlayer);

/*
	function which checks if command is spelled correctly,
	then calls adequate function to deal with specific command
*/
void handle_player_command(char command[], struct board_t** board, struct player_t** players,
	struct dim_t* dim, int playerId, int* shots, int extendedShips);

void handle_player_ends_turn(char command[], int commandId, struct player_t** players, int* activeCommandType,
	int* currentPlayer, int* nextPlayer, int* shots);

void handle_all_commands(int commandId, int* quit, int* activeCommandType, char command[], int* nextPlayer,
	struct board_t** board, struct player_t** players, struct dim_t* dim, int* extendedShips, struct vector_t* savedCommands, struct vector_t* reefs,
	int* seed, int* currentPlayer, int* shots, int* aiPlayer, int* aiMoved);

void handle_changing_player_type(char command[], int* nextPlayer, int* commandId,
	int* currentPlayer, int* activeCommandType);
