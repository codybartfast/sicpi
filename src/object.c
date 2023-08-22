#include "object.h"

#include "sicp-error.h"
#include "sicp-std.h"

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum value_kind {
	VK_ERROR = -2,
	VK_EOS = -1,
	VK_UNDEFINED = 0,
	VK_INTEGER,
	VK_FLOATING
} value_kind;

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

static int8_t object_value_kind(object obj)
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

void check_value_kind(object obj, value_kind kind, char *caller)
{
	if (object_value_kind(obj) != kind) {
		inyim("%s given wrong value kind. Expected %d, given %d.",
		      caller, object_value_kind(obj), kind);
	}
}

//
// Error
// =============================================================================
//

inline bool is_error(object obj)
{
	return object_value_kind(obj) == VK_ERROR;
}

inline object from_error_kind(enum error_kind error_kind, meta_data meta_data)
{
	return object_new(VK_ERROR, meta_data,
			  (value_union){ .error_kind = error_kind });
}

inline enum error_kind to_error_kind(object obj)
{
	check_value_kind(obj, VK_ERROR, "to_error_kind");
	return obj->value.error_kind;
}

//
// Number
// =============================================================================
//

inline bool is_number(object obj)
{
	value_kind vk = object_value_kind(obj);
	return vk == VK_INTEGER || vk == VK_FLOATING;
}

inline object from_integer(integer integer, meta_data meta_data)
{
	return object_new(VK_INTEGER, meta_data,
			  (value_union){ .integer = integer });
}

inline integer to_integer(object obj)
{
	check_value_kind(obj, VK_INTEGER, "to_integer");
	return obj->value.integer;
}

inline object from_floating(floating floating, meta_data meta_data)
{
	return object_new(VK_FLOATING, meta_data,
			  (value_union){ .floating = floating });
}

inline floating to_floating(object obj)
{
	check_value_kind(obj, VK_FLOATING, "to_floating");
	return obj->value.floating;
}
