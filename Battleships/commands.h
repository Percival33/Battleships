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
	struct player_t** player, struct dim_t* dim, int* extendedShips);

/*
	function which checks if command is spelled correctly,
	then calls adequate function to deal with specific command
*/
void handle_player_command(char command[], struct board_t** board, struct player_t** players, struct dim_t* dim, int playerId, int* shots, int extendedShips);