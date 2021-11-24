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

const char SHIP_NOT_ON_POSITION_CHAR[] = "NOT IN STARTING POSITION";
const char SHIP_PRESENT_CHAR[] = "SHIP ALREADY PRESENT";
const char SHIP_ALL_SET_CHAR[] = "ALL SHIPS OF THE CLASS ALREADY SET";
const char OTHER_PLAYER_EXPECTED_CHAR[] = "THE OTHER PLAYER EXPECTED";

const int dx[] = { 0, -1, 0, 1};  
const int dy[] = { 1, 0, -1, 0};

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
