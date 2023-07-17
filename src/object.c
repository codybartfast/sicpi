#include "object.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum value_kind { INTEGER = 1 } value_kind;

// Construction
// =============================================================================
//

static object object_new(uint8_t value_kind, meta_data meta_data,
			 value_union value)
{
	object obj = malloc(sizeof(struct object));
	if (!obj) {
		alloc_error("object_new");
	}
	obj->value_kind = value_kind;
	obj->meta_data = meta_data;
	obj->value = value;

	return obj;
}

// General
// =============================================================================
//

static uint8_t object_value_kind(object obj)
{
	return obj->value_kind;
}

meta_data object_meta_data(object obj)
{
	return obj->meta_data;
}

// Number
// =============================================================================
//

bool is_number(object obj)
{
	value_kind vk = object_value_kind(obj);
	return vk == INTEGER;
}

object from_integer(integer integer, meta_data meta_data)
{
	return object_new(INTEGER, meta_data,
			  (value_union){ .integer = integer });
}

integer to_integer(object obj)
{
	return obj->value.integer;
}