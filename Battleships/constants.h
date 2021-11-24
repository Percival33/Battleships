#pragma once

/*
	TODO: add const to specify:
			- type of ship
			- direction of ship
			- whether ship was hit
			- 
*/

const char STATE[];
const char PLAYER_A[];
const char PLAYER_B[];
const char PLACE_SHIP[];

const char SHIP_NOT_ON_POSITION[];
const char SHIP_PRESENT[];
const char SHIP_ALL_SET[];
const char OTHER_PLAYER_EXPECTED[];

const int dx[]; 
const int dy[];

/*
	Initialize with { y , x }
*/
typedef struct field{
	int y;
	int x;
}field_t;

enum Consts {
	True = 1,
	False = 0
};

enum Commands { 
	
	C_NULL = -1,					// Current command is null
	C_QUIT = 'Q',

	C_STATE = 0,				// state command ID
	C_PLAYER_A = 1,			// player A command ID
	C_PLAYER_B = 2,			// player B command ID
	C_INVALID = 3,			// Invalid command ID
	C_PLACE_SHIP = 4,			// Place ship command

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
	B_TAKEN = 201
};


int get_dir(char dirChar);

int get_class(char cat[]);
