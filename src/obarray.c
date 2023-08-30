#include "obarray.h"

#include "sicp-std.h"

/*
 * Code here has been copied from string_builder,
 * so any fixes/mods here might be wanted there too.
 */

void obarray_init(obarray obarray, size_t initial_capacity)
{
	initial_capacity = initial_capacity < 1 ?
				   TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY :
				   initial_capacity;
	char **start =
		mallocx(sizeof(char **) * initial_capacity, "obarray_init");
	obarray->start = start;
	obarray->next = start;
	obarray->end = start + initial_capacity;
}

obarray obarray_new(size_t initial_capacity)
{
	obarray oba = mallocx(sizeof(struct obarray), "obarray_new");
	obarray_init(oba, initial_capacity);
	return oba;
}

char *obarray_intern(char *name)
{
	unused(name);
	return NULL;
}

void obarray_free_members(obarray obarray)
{
	free(obarray->start);
	obarray->start = NULL;
	obarray->next = NULL;
	obarray->end = NULL;
}

void obarray_free(obarray obarray)
{
	obarray_free_members(obarray);
	free(obarray);
}
