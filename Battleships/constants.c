#include "constants.h"

const char STATE_CHAR[] = "[state]";
const char PLAYER_A_CHAR[] = "[playerA]";
const char PLAYER_B_CHAR[] = "[playerB]";
const char WRONG_COMMAND_CHAR[] = "WRONG COMMAND";

const char PRINT_CHAR[] = "PRINT";
const char SET_FLEET_CHAR[] = "SET_FLEET";
const char NEXT_PLAYER_CHAR[] = "NEXT_PLAYER";
const char PLACE_SHIP_CHAR[] = "PLACE_SHIP";
const char SHOOT_CHAR[] = "SHOOT";

const char NOT_IN_STARTING_POSITION_CHAR[] = "NOT IN STARTING POSITION";
const char SHIP_ALREADY_PRESENT_CHAR[] = "SHIP ALREADY PRESENT";
const char ALL_SHIPS_OF_THE_CLASS_ALREADY_SET_CHAR[] = "ALL SHIPS OF THE CLASS ALREADY SET";
const char SHIP_NOT_ALL_SHIPS_PLACED_CHAR[] = "NOT ALL SHIPS PLACED";
const char OTHER_PLAYER_EXPECTED_CHAR[] = "THE OTHER PLAYER EXPECTED";

const char FIELD_DOES_NOT_EXIST_CHAR[] = "FIELD DOES NOT EXIST";

const char SHIP_CHAR[] = "SHIP";
const char BOARD_SIZE_CHAR[] = "BOARD_SIZE";
const char INIT_POSITION_CHAR[] = "INIT_POSITION";
const char REEF_CHAR[] = "REEF";
const char REEF_IS_NOT_PLACED_ON_BOARD_CHAR[] = "REEF IS NOT PLACED ON BOARD";
const char PLACING_SHIP_ON_REEF_CHAR[] = "PLACING SHIP ON REEF";
const char PLACING_SHIP_TOO_CLOSE_TO_OTHER_SHIP_CHAR[] = "PLACING SHIP TOO CLOSE TO OTHER SHIP";

const int dx[] = { 0, -1, 0, 1};  
const int dy[] = { 1, 0, -1, 0};

/*
const int MAX_SHIPS_NUMBER = 10;
const int MAX_SHIP_LENGTH = 5;
const int MAX_SHIP_TYPE_NUMBER = 6;
*/


int get_dir(char dirChar) {
	int dir;
	switch ((int)dirChar)
	{
	case 'N':
		dir = N;
		break;
	case 'E':
		dir = E;
		break;
	case 'S':
		dir = S;
		break;
	case 'W':
		dir = W;
		break;
	default:
		dir = -1;
		break;
	}
	return dir;
}

int is_equal(char A[], char B[]) {
	if (strcmp(A, B) == 0) {
		return True;
	}
	return False;
}

int get_class(char catChar[]) {
	int cat = -1;
	if (is_equal(catChar, "CAR")) {
		cat = S_CAR;
	}
	if (is_equal(catChar, "BAT")) {
		cat = S_BAT;
	}
	if (is_equal(catChar, "CRU")) {
		cat = S_CRU;
	}
	if(is_equal(catChar, "DES")) {
		cat = S_DES;
	}

	return cat;
}

int get_player_id(char P) {
	if (P == 'A')
		return PLAYER_A;
	else if (P == 'B')
		return PLAYER_B;
	return -1;
}
