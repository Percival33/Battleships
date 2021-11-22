#pragma once
/*
	TODO: add const to specify:
			- type of ship
			- direction of ship
			- whether ship was hit
			- 
*/

typedef struct {
	int x;
	int y;
}point_t;

enum Consts {
	True = 1,
	False = 0
};

enum Commands { // values from 100 except 'Q' and C_NULL value
	
	C_NULL = 0,					// Current command is null
	C_QUIT = 'Q',
	C_STATE = 100,				// state command ID
	C_PLAYER_A = 101,			// player A command ID
	C_PLAYER_B = 102,			// player B command ID
	C_INVALID = 103,			// Invalid command ID
	C_IN = 104,					// Switching command type 
	C_OUT = 105,				// Switching command type
	C_CORRECT = 106,			// Correct command
	C_PLACE_SHIP = 107,			// Place ship command
	
};

enum Ship { // values from 2 to 11
	S_CAR = 5,					// [CAR]RIER size
	S_BAT = 4,					// [BAT]TLESHIP size
	S_CRU = 3,					// [CRU]ISER size
	S_DES = 2,					// [DES]TROYER size

	S_PLACE = 10,				// Place ship command ID
	S_SHOOT = 11,				// Shoot command ID
};

enum Direction { // values from 0 to 3
	N = 0,
	E = 1,
	S = 2,
	W = 3
};

enum Board { // values from 200 to 201
	B_EMPTY = 200,
	B_TAKEN = 201
};