#pragma once
#include "constants.h"
/*
	TODO: create ship struct 
*/

typedef struct {
	int type;
	point_t head;
	int direction;
	int damaged; // should be array
}ship_t;