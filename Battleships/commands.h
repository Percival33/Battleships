#pragma once
/*
	function reads command from input and returns its ID
*/
int get_command_type();

/*
	function prints INVALID INPUT and ID of activeCommand
	TODO: prints improperly written command
*/
void handle_invalid_command(char command[], int* activeCommand);


/*
	function prints whether command starts new type of commands or ends it.
	If command is invalid then calls function handle_invalid_command 
	TODO: return value of current type of commands
*/
void handle_command(char command[], int* activeCommand, int type);