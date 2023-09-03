#include "object.h"

#include "sicp-error.h"
#include "sicp-std.h"

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

enum value_kind {
	VK_ERROR = -1,
	VK_UNDEFINED = 0,
	VK_SINGLETON,
	VK_INTEGER,
	VK_FLOATING,
	VK_STRING,
	VK_SYMBOL,
	VK_PAIR
};

//
// Helpers
// =============================================================================
//

static inline int8_t object_value_kind(object obj)
{
	return obj->value_kind;
}

static inline void check_value_kind(object obj, enum value_kind kind,
				    char *caller)
{
	if (object_value_kind(obj) != kind) {
		inyim("%s given wrong value kind. Expected %d, given %d.",
		      caller, object_value_kind(obj), kind);
	}
}

//
// Construction
// =============================================================================
//

static object object_new(int8_t value_kind, meta_data meta_data,
			 object_value value)
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

meta_data object_meta_data(object obj)
{
	return obj->meta_data;
}

void object_free(object obj)
{
	enum value_kind kind = object_value_kind(obj);
	switch (kind) {
	case VK_STRING:
		free((char *)obj->value.string);
	default:
		break;
	}
	if (kind != VK_SYMBOL) {
		free(obj);
	}
}

inline bool eq(object x, object y)
{
	return x == y;
}

//
// Errors
// =============================================================================
//

inline bool is_error(object obj)
{
	return object_value_kind(obj) == VK_ERROR;
}

inline object from_error_kind(enum error_kind error_kind, meta_data meta_data)
{
	return object_new(VK_ERROR, meta_data,
			  (object_value){ .error_kind = error_kind });
}

inline enum error_kind to_error_kind(object obj)
{
	check_value_kind(obj, VK_ERROR, "to_error_kind");
	return obj->value.error_kind;
}

//
// Singletons
// =============================================================================
//

#define SINGLETON(TEXT)                                                        \
	{                                                                      \
		VK_SINGLETON, NO_META_DATA,                                    \
		{                                                              \
			.string = TEXT                                         \
		}                                                              \
	}

bool is_singleton(const object obj)
{
	return object_value_kind(obj) == VK_SINGLETON;
}

const char *to_singleton_text(const object obj)
{
	check_value_kind(obj, VK_SINGLETON, "to_singleton");
	return obj->value.string;
}

static struct object _Empty_List = SINGLETON("'()");
const object Empty_List = &_Empty_List;

static struct object _Eos = SINGLETON("end_of_source");
const object Eos = &_Eos;

//
// Numbers
// =============================================================================
//

inline bool is_number(object obj)
{
	enum value_kind vk = object_value_kind(obj);
	return vk == VK_INTEGER || vk == VK_FLOATING;
}

inline object from_integer(integer integer, meta_data meta_data)
{
	return object_new(VK_INTEGER, meta_data,
			  (object_value){ .integer = integer });
}

inline integer to_integer(object obj)
{
	check_value_kind(obj, VK_INTEGER, "to_integer");
	return obj->value.integer;
}

inline object from_floating(floating floating, meta_data meta_data)
{
	return object_new(VK_FLOATING, meta_data,
			  (object_value){ .floating = floating });
}

inline floating to_floating(object obj)
{
	check_value_kind(obj, VK_FLOATING, "to_floating");
	return obj->value.floating;
}

//
// Strings
// =============================================================================
//

inline bool is_string(object obj)
{
	return object_value_kind(obj) == VK_STRING;
}

inline object from_string(char *string, meta_data meta_data)
{
	return object_new(VK_STRING, meta_data,
			  (object_value){ .string = string });
}

inline char const *to_string(object obj)
{
	check_value_kind(obj, VK_STRING, "to_string");
	return obj->value.string;
}

//
// Pairs
// =============================================================================
//

bool is_pair(const object obj)
{
	return object_value_kind(obj) == VK_PAIR;
}

object cons(const object a, const object b, meta_data meta_data)
{
	return object_new(VK_PAIR, meta_data,
			  (object_value){ .pair = { a, b } });
}

object car(const object obj)
{
	check_value_kind(obj, VK_PAIR, "to_car");
	return obj->value.pair.car;
}

object cdr(const object obj)
{
	check_value_kind(obj, VK_PAIR, "to_cdr");
	return obj->value.pair.cdr;
}

object set_car(object pair, object new_car)
{
	pair->value.pair.car = new_car;
	return pair;
}

object set_cdr(object pair, object new_cdr)
{
	pair->value.pair.cdr = new_cdr;
	return pair;
}

//
// Symbols
// =============================================================================
//

inline bool is_symbol(object obj)
{
	return object_value_kind(obj) == VK_SYMBOL;
}

inline object from_name(char *id, meta_data meta_data)
{
	return object_new(VK_SYMBOL, meta_data, (object_value){ .string = id });
}

inline char const *to_name(object obj)
{
	check_value_kind(obj, VK_SYMBOL, "to_id");
	return obj->value.string;
}
