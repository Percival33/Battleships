#pragma once
#include "constants.h"

typedef struct text{
	char text[MAX_COMMAND_LENGTH];
} text_t;

typedef struct {
	int allocated_size;
	int count;
	text_t* ptr;
} vector_t;

void init(vector_t* v);

void reallocate(vector_t* v, int new_size);

void push_back(vector_t* v, char* command);

char* pop_back(vector_t* v);
