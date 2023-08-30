#include "sicp-error.h"
#include "obarray.h"
#include "sicp-std.h"

#include <string.h>

#define GROWTH_FACTOR 2

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

static void grow(obarray oba)
{
	static size_t element_size = sizeof(char **);
	static size_t sizeLimit = __SIZE_MAX__;
	static size_t fullGrowLimit = __SIZE_MAX__ / GROWTH_FACTOR;
	size_t old_alloc = element_size * (oba->end - oba->start);

	if (old_alloc == sizeLimit) {
		alloc_error("obarray:grow (limit)");
	}
	size_t new_alloc = old_alloc > fullGrowLimit ?
				   sizeLimit :
				   old_alloc * GROWTH_FACTOR;
	char **new_buff = realloc(oba->start, new_alloc);
	if (!new_buff) {
		alloc_error("obarray:grow realloc");
	}
	char **new_next_write = new_buff + (oba->next - oba->start);

	oba->start = new_buff;
	oba->next = new_next_write;
	oba->end = new_buff + (new_alloc / element_size);
}

char *add(obarray oba, char *name)
{
	if (oba->next >= oba->end) {
		grow(oba);
	}
	*oba->next = name;
	++oba->next;
	return name;
}

char *obarray_intern(obarray obarray, char *name)
{
	char **i;
	for (i = obarray->start; i < obarray->next; i++) {
		if (strcmp(name, *i) == 0) {
			return *i;
		}
	}
	return add(obarray, strdupx(name, "obarray_intern"));
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
