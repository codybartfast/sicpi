#ifndef OBARRAY_H
#define OBARRAY_H

#include <stdlib.h>

typedef struct obarray {
	char **strings;
	char **next;
	char **table_end;
} *obarray;

obarray obarray_new(size_t initial_capacity);

char *obarray_intern(char *string);

void obarray_free(obarray obarray);

#endif
