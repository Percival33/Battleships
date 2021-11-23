#pragma once

/*
	function reads command from input and returns its ID
	TODO: merge it with handle command type
*/
int get_command_type(char command[]);

/*
	function handles invalid command, calls print_invalid_command and exits program
	TODO: write body of function
*/
void handle_invalid_command(char command[], int activeCommandType);


/*
	function prints whether command is correctly spelled.
	If not then calls function handle_invalid_command 
	TODO: return value of current type of commands
*/
int handle_command_type(char command[], int* activeCommand, int type);
