#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "constants.h"

void init(vector_t* v) {
	v->count = 0;
	v->allocated_size = 1;
	v->ptr = (text_t*)malloc(v->allocated_size * sizeof(text_t));
}

void reallocate(vector_t* v, int new_size) {
	v->allocated_size = new_size;
	text_t* newPtr = (text_t*)malloc(v->allocated_size * sizeof(text_t));
	memcpy(newPtr, v->ptr, v->count * sizeof(text_t));
	free(v->ptr);
	v->ptr = newPtr;
	return;
}

void push_back(vector_t* v, char* command) {
	if (v->count + 1 == v->allocated_size) { // no place for new element
		reallocate(v, 2 * v->allocated_size);
	}
	text_t text;
	strcpy(text.text, command);
	v->ptr[v->count] = text;
	v->count++;
	return;
}

void print_self(vector_t* v) {
	for (int i = 0; i < v->count; i++) {
		printf("%s \n", v->ptr[i].text);
	}
}