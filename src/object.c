#include "object.h"

#include "obarray.h"
#include "sicp-error.h"
#include "sicp-std.h"

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

enum value_kind {
	VK_ERROR = -1,
	VK_UNDEFINED = 0,
	VK_SINGLETON, // 1
	VK_INTEGER, //// 2
	VK_FLOATING, /// 3
	VK_STRING, ///// 4
	VK_SYMBOL, ///// 5
	VK_PAIR //////// 6
};

//
// Helpers
// =============================================================================
//

static char *value_kind_name(enum value_kind value_kind)
{
	switch (value_kind) {
	case VK_ERROR:
		return "Error";
	case VK_UNDEFINED:
		return "Undefined";
	case VK_SINGLETON:
		return "Global Constant";
	case VK_INTEGER:
		return "Integer";
	case VK_FLOATING:
		return "Floating Point Number";
	case VK_STRING:
		return "String";
	case VK_SYMBOL:
		return "Symbol";
	case VK_PAIR:
		return "Pair";
	default:
		inyim("Don't know name of value kind: %d", value_kind);
		exit(1); // keep compiler happy
	}
}

static inline int8_t object_value_kind(object obj)
{
	return obj->value_kind;
}

static inline void check_value_kind(object obj, enum value_kind expected,
				    char *caller)
{
	enum value_kind actual = object_value_kind(obj);
	if (actual != expected) {
		inyim("'%s' given a %s, but expected a %s.", caller,
		      value_kind_name(actual), value_kind_name(expected));
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

static struct object _EMPTY_LIST = SINGLETON("'()");
const object EMPTY_LIST = &_EMPTY_LIST;

static struct object _EOS = SINGLETON("end_of_source");
const object EOS = &_EOS;

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

bool is_null(const object obj)
{
	return obj == EMPTY_LIST;
}

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
	check_value_kind(obj, VK_PAIR, "car");
	return obj->value.pair.car;
}

object cdr(const object obj)
{
	check_value_kind(obj, VK_PAIR, "cdr");
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

#define SYMBOL(TEXT)                                                           \
	{                                                                      \
		VK_SYMBOL, NO_META_DATA,                                       \
		{                                                              \
			.string = TEXT                                         \
		}                                                              \
	}

obarray symbols = NULL;

inline bool is_symbol(object obj)
{
	return object_value_kind(obj) == VK_SYMBOL;
}

object create_obarray_entry(char *name, meta_data meta_data)
{
	return object_new(VK_SYMBOL, meta_data,
			  (object_value){ .string = name });
}

void init_keywords(void);

inline object from_name(char *name, meta_data meta_data)
{
	if (!symbols) {
		init_keywords();
	}
	return obarray_intern(symbols, name, meta_data);
}

inline char const *to_name(object obj)
{
	check_value_kind(obj, VK_SYMBOL, "to_name");
	return obj->value.string;
}

//
// Symbols - Keywords
// =============================================================================
//

static struct object _DOT = SYMBOL(".");
const object DOT = &_DOT;

static struct object _QUASIQUOTE = SYMBOL("quasiquote");
const object QUASIQUOTE = &_QUASIQUOTE;

static struct object _QUOTE = SYMBOL("quote");
const object QUOTE = &_QUOTE;

static struct object _UNQUOTE = SYMBOL("unquote");
const object UNQUOTE = &_UNQUOTE;

// Above keywords need to be included function below

void init_keywords(void)
{
	symbols = obarray_new(0);

	obarray_add_symbol(symbols, DOT);
	obarray_add_symbol(symbols, QUASIQUOTE);
	obarray_add_symbol(symbols, QUOTE);
	obarray_add_symbol(symbols, UNQUOTE);
}
