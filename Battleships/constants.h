#pragma once

const char STATE_CHAR[];
const char PLAYER_A_CHAR[];
const char PLAYER_B_CHAR[];
const char WRONG_COMMAND_CHAR[];

const char PRINT_CHAR[];
const char SET_FLEET_CHAR[];
const char NEXT_PLAYER_CHAR[];
const char PLACE_SHIP_CHAR[];
const char SHOOT_CHAR[];
const char MOVE_CHAR[];

const char NOT_IN_STARTING_POSITION_CHAR[];
const char SHIP_ALREADY_PRESENT_CHAR[];
const char ALL_SHIPS_OF_THE_CLASS_ALREADY_SET_CHAR[];
const char SHIP_NOT_ALL_SHIPS_PLACED_CHAR[];
const char OTHER_PLAYER_EXPECTED_CHAR[];

const char FIELD_DOES_NOT_EXIST_CHAR[];

const char SHIP_CHAR[];
const char BOARD_SIZE_CHAR[];
const char INIT_POSITION_CHAR[];
const char REEF_CHAR[];
const char REEF_IS_NOT_PLACED_ON_BOARD_CHAR[];
const char PLACING_SHIP_ON_REEF_CHAR[];
const char PLACING_SHIP_TOO_CLOSE_TO_OTHER_SHIP_CHAR[];
const char SHIP_MOVED_ALREADY_CHAR[];
const char SHIP_CANNOT_MOVE_CHAR[];
const char SHIP_WENT_FROM_BOARD_CHAR[];
const char SHIP_CANNOT_SHOOT_CHAR[];
const char TOO_MANY_SHOOTS_CHAR[];
const char SHOOTING_TOO_FAR_CHAR[];


const int dx[]; 
const int dy[];

/*
	Initialize with { y , x }
*/
typedef struct field{
	int y;
	int x;
}field_t;

typedef enum Bool {
	True = 1,
	False = 0
} bool;

enum Consts {
	MAX_SHIPS_NUMBER = 10,
	MAX_SHIP_LENGTH = 5,
	MAX_SHIP_TYPE_NUMBER = 6,
	DEFAULT_ROWS_NUMBER = 21,
	DEFAULT_COLS_NUMBER = 10,
	MAX_COMMAND_LENGTH = 105,
	CAR_MOVES = 2,
	OTHER_MOVES = 3,
};

enum Commands {
	ERROR = 2137, 
	C_NULL = -1,					// Current command is null
	C_QUIT = 'Q',

	C_STATE = 10,				// state command ID
	C_PLAYER_TYPE = 999,
	C_STATE_TYPE = 998,
	C_INVALID = 3,			// Invalid command ID
	
	C_PLACE_SHIP = 4,			// Place ship command
	C_PRINT = 5,			// Place ship command
	C_SET_FLEET = 6,			// Place ship command
	C_NEXT_PLAYER = 7,			// Place ship command
	C_SHOOT = 8,			// Place ship command
	C_SHIP = 9,
	C_BOARD_SIZE = 10,

	C_NOT_ALL_SHIPS_PLACED = 1000,
	C_FIELD_DOES_NOT_EXIST = 1001,
	C_NOT_IN_STARTING_POSITION = 1002,
	C_SHIP_ALREADY_PRESENT = 1003,
	C_ALL_SHIPS_OF_THE_CLASS_ALREADY_SET = 1004,
	C_INIT_POSITION = 1005,
	C_REEF_NOT_ON_BOARD = 1006,
	C_SHIP_ON_REEF = 1007,
	C_PLACING_SHIP_TOO_CLOSE = 1008,
	C_PLACING_SHIP_ON_REEF = 1009,
	C_SHIP_MOVED_ALREADY = 1010,
	C_SHIP_CANNOT_MOVE = 1011,
	C_SHIP_WENT_FROM_BOARD = 1012,
	C_EXTENDED = 1013,
	C_SHIP_CANNOT_SHOOT = 1014,
	C_TOO_MANY_SHOOTS = 1015,
	C_SHOOTING_TOO_FAR = 1016,


	C_IN = 104,					// Switching command type 
	C_OUT = 105,				// Switching command type
	C_CORRECT = 106,			// Correct command
	C_WRONG_ARGS = 107,

};

enum Ship {
	S_CAR = 5,					// [CAR]RIER size
	S_BAT = 4,					// [BAT]TLESHIP size
	S_CRU = 3,					// [CRU]ISER size
	S_DES = 2,					// [DES]TROYER size
	S_NULL = -1,
};

enum Direction { 
	N = 0,
	E = 1,
	S = 2,
	W = 3, 

	F = 0,			// [F]orward
	L = -1,			// [L]eft
	R = 1,			// [R]ight
};

enum Board {
	B_EMPTY = 200,
	B_BAN = 201,
	B_REEF = 202,

	B_TAKEN = 203,
	B_ENGINE = 204,
	B_CANNON = 205,
	B_RADAR = 206,
	B_DESTROYED = 207,
};

enum Player {
	PLAYER_A = 0,
	PLAYER_B = 1,
};


int get_dir(char dirChar);

int get_class(char cat[]);

int get_player_id(char P);

int get_move_dir(char dir);