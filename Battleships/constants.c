#include "constants.h"

const char STATE[] = "[state]";
const char PLAYER_A[] = "[playerA]";
const char PLAYER_B[] = "[playerB]";
const char WRONG_COMMAND[] = "WRONG COMMAND";

const char PRINT[] = "PRINT";
const char SET_FLEET[] = "SET_FLEET";
const char NEXT_PLAYER[] = "NEXT_PLAYER";
const char PLACE_SHIP[] = "PLACE_SHIP";
const char SHOOT[] = "SHOOT";

const char SHIP_NOT_ON_POSITION[] = "NOT IN STARTING POSITION";
const char SHIP_PRESENT[] = "SHIP ALREADY PRESENT";
const char SHIP_ALL_SET[] = "ALL SHIPS OF THE CLASS ALREADY SET";
const char OTHER_PLAYER_EXPECTED[] = "THE OTHER PLAYER EXPECTED";

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
