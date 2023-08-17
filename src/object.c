#include "object.h"

#include "sicp-error.h"
#include "sicp-std.h"

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum value_kind { INTEGER = 1 } value_kind;

//
// Construction
// =============================================================================
//

static object object_new(uint8_t value_kind, meta_data meta_data,
			 value_union value)
{
	struct object tmp = { .value_kind = value_kind,
			      .meta_data = meta_data,
			      .value = value };
	size_t size = sizeof(struct object);
	object obj = malloc(size);
	if (!obj) {
		alloc_error("object_new");
	}
	memcpy(obj, &tmp, size);
	return obj;
}

//
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

void object_free(object obj)
{
	switch (object_value_kind(obj)) {
	default:
		break;
	}
	free(obj);
}

//
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
