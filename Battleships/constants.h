#pragma once

/*
	TODO: add const to specify:
			- type of ship
			- direction of ship
			- whether ship was hit
			- 
*/

const char STATE_CHAR[];
const char PLAYER_A_CHAR[];
const char PLAYER_B_CHAR[];
const char WRONG_COMMAND_CHAR[];

const char PRINT_CHAR[];
const char SET_FLEET_CHAR[];
const char NEXT_PLAYER_CHAR[];
const char PLACE_SHIP_CHAR[];
const char SHOOT_CHAR[];

const char SHIP_NOT_ON_POSITION_CHAR[];
const char SHIP_PRESENT_CHAR[];
const char SHIP_ALL_SET_CHAR[];
const char OTHER_PLAYER_EXPECTED_CHAR[];

const int dx[]; 
const int dy[];

const int SHIP_MAX_NUMBER;

/*
	Initialize with { y , x }
*/
typedef struct field{
	int y;
	int x;
}field_t;

enum Consts {
	True = 1,
	False = 0,
	PLAYER_A = 0,
	PLAYER_B = 1
};

enum Commands {

	C_NULL = -1,					// Current command is null
	C_QUIT = 'Q',

	C_STATE = 0,				// state command ID
	C_PLAYER_A = 1,			// player A command ID
	C_PLAYER_B = 2,			// player B command ID
	C_PLAYER_TYPE = 999,
	C_STATE_TYPE = 998,
	C_INVALID = 3,			// Invalid command ID
	
	C_PLACE_SHIP = 4,			// Place ship command
	C_PRINT = 5,			// Place ship command
	C_SET_FLEET = 6,			// Place ship command
	C_NEXT_PLAYER = 7,			// Place ship command
	C_SHOOT = 8,			// Place ship command


	C_IN = 104,					// Switching command type 
	C_OUT = 105,				// Switching command type
	C_CORRECT = 106,			// Correct command

};

enum Ship {
	S_CAR = 5,					// [CAR]RIER size
	S_BAT = 4,					// [BAT]TLESHIP size
	S_CRU = 3,					// [CRU]ISER size
	S_DES = 2,					// [DES]TROYER size

	S_PLACE = 10,				// Place ship command ID
	S_SHOOT = 11,				// Shoot command ID
};

enum Direction { 
	N = 0,
	E = 1,
	S = 2,
	W = 3
};

enum Board {
	B_EMPTY = 200,
	B_TAKEN = 201,
	B_DESTROYED = 202,
	B_BAN = 203,
};


int get_dir(char dirChar);

int get_class(char cat[]);
