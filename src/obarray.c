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
				   OBARRAY_DEFAULT_INITIAL_CAPACITY :
				   initial_capacity;
	object *start =
		mallocx(sizeof(object *) * initial_capacity, "obarray_init");
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
	static size_t element_size = sizeof(object *);
	static size_t sizeLimit = __SIZE_MAX__;
	static size_t fullGrowLimit = __SIZE_MAX__ / GROWTH_FACTOR;
	size_t old_alloc = element_size * (oba->end - oba->start);

	if (old_alloc == sizeLimit) {
		alloc_error("obarray:grow (limit)");
	}
	size_t new_alloc = old_alloc > fullGrowLimit ?
				   sizeLimit :
				   old_alloc * GROWTH_FACTOR;
	object *new_buff = realloc(oba->start, new_alloc);
	if (!new_buff) {
		alloc_error("obarray:grow realloc");
	}
	object *new_next_write = new_buff + (oba->next - oba->start);

	oba->start = new_buff;
	oba->next = new_next_write;
	oba->end = new_buff + (new_alloc / element_size);
}

object add(obarray oba, char *name)
{
	if (oba->next >= oba->end) {
		grow(oba);
	}
	object symbol = from_name(name, NO_META_DATA);
	*oba->next = symbol;
	++oba->next;
	return symbol;
}

object obarray_intern(obarray obarray, char *name)
{
	object *obj_ptr;
	for (obj_ptr = obarray->start; obj_ptr < obarray->next; obj_ptr++) {
		if (strcmp(name, to_name(*obj_ptr)) == 0) {
			return *obj_ptr;
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
