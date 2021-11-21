#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	int type;
} board_t;

board_t** create_array(const int rows, const int cols);

void delete_array(board_t** arr, const int rows);