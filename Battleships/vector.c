#include <stdlib.h>
#include "vector.h"
#include "constants.h"

void init(vector_t* v) {
	v->count = 0;
	v->allocated_size = 1;
	v->ptr = (text_t*)malloc(v->allocated_size * sizeof(text_t));
}

void reallocate(vector_t* v, int new_size) {
	v->allocated_size = new_size;
	char* newPtr = (text_t*)malloc(v->allocated_size * sizeof(text_t));
	memcpy(newPtr, v->ptr, v->count * sizeof(text_t));
	free(v->ptr);
	v->ptr = newPtr;
	return;
}

void push_back(vector_t* v, char command[]) {
	if (v->count == v->allocated_size) { // no place for new element
		reallocate(v, 2 * v->allocated_size);
	}
	text_t text;
	strcpy(text.text, command);
	v->ptr[v->count] = text;
	v->count++;
	return;
}
