#ifndef OBARRAY_H
#define OBARRAY_H

#include <stdlib.h>

#define TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY 1024

typedef struct obarray {
	char **start;
	char **next;
	char **end;
} *obarray;

void obarray_init(obarray obarray, size_t initial_capacity);
obarray obarray_new(size_t initial_capacity);

char *obarray_intern(obarray obarray, char *name);

void obarray_free_members(obarray obarray);
void obarray_free(obarray obarray);

#endif
