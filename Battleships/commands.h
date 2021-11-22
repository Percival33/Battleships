#pragma once



/*
	function reads command from input and returns its ID
*/
int get_command_type(char command[]);

/*
	function prints INVALID COMMAND, TAG of command and reason why it is invalid
	TODO: prints improperly written command
*/
void handle_invalid_command(char command[], char tag[], char reason[]);


/*
	function prints whether command is correctly spelled.
	If not then calls function handle_invalid_command 
	TODO: return value of current type of commands
*/
int handle_command_type(char command[], int* activeCommand, int type);

/*
	function handles place_ship command, if it is correct then executes it and C_CORRECT
	otherwise returns C_INVALID

*/
int handle_command(char command[]);
