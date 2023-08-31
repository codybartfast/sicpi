// https://www.sicp-book.com/book-Z-H-33.html#%_idx_5886

#ifndef OBARRAY_H
#define OBARRAY_H

#include "object.h"

#include <stdlib.h>

#define TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY 1024

typedef struct obarray {
	object *start;
	object *next;
	object *end;
} *obarray;

void obarray_init(obarray obarray, size_t initial_capacity);
obarray obarray_new(size_t initial_capacity);

object obarray_intern(obarray obarray, char *name);

void obarray_free_members(obarray obarray);
void obarray_free(obarray obarray);

#endif
