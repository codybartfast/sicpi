#ifndef OBJECT_H
#define OBJECT_H

#include "sicp-std.h"

#include <stdbool.h>
#include <inttypes.h>

/*
 *value_kind is stored in a int_8, so max value is 127.
 */

enum value_kind {
	VK_ERROR = -1,
	VK_UNDEFINED = 0,
	VK_SINGLETON, //////////// 1
	VK_INTEGER, ////////////// 2
	VK_FLOATING, ///////////// 3
	VK_STRING, /////////////// 4
	VK_SYMBOL, /////////////// 5
	VK_PAIR, ///////////////// 6
	VK_PRIMITIVE_PROCEDURE, // 7
	VK_GOTO_LABEL //////////// 8
};

enum error_kind {
	ERROR_UNSPECIFIED = 0,
	ERROR_DIVISION_BY_ZERO,
	EMPTY_BEGIN_SEQUENCE,
	ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
	ERROR_LEXER,
	ERROR_PARSER,
	ERROR_UNBOUND_VARIABLE,
	ERROR_UNEXPECTED_TYPE,
	ERROR_UNKNOWN_EXPRESSION_TYPE,
	ERROR_UNKNOWN_PROCEDURE_TYPE
};

struct object;

typedef struct pair {
	struct object *car;
	struct object *cdr;
} *pair;

typedef union object_value {
	const floating floating;
	const integer integer;
	const enum error_kind error_kind;
	const char *string;
	struct object *(*primitive_procedure)(struct object *);

	// This is the largest type, 2 * 64b = 128b.
	// Perhaps, if/when 'storage' is implemented it could be replaced
	// by 2 * 32b addresses into the starage array?
	struct pair pair;
} object_value;

typedef struct object {
	const int8_t value_kind;
	const meta_data meta_data;
	object_value value;
} *object;

//
// Construction
// =============================================================================
//

void object_free(object obj);

//
// Helpers
// =============================================================================
//

int8_t object_value_kind(object obj);

//
// General
// =============================================================================
//

meta_data object_meta_data(const object obj);
char *to_text(object obj);

//
// Equality
// =============================================================================
//

bool is_eq(const object a, const object b);

//
// Errors
// =============================================================================
//

bool is_error(const object obj);
object of_error_kind(enum error_kind error_kind, meta_data meta_data);
enum error_kind to_error_kind(const object obj);
char *error_kind_name(enum error_kind error_kind);

//
// Singletons (Global Constants)
// =============================================================================
//

extern const object EMPTY_LIST;
bool is_null(const object obj);

extern const object EOS;
bool is_eos(object obj);

extern const object FALSE_VALUE;
bool is_false(object obj);

extern const object TRUE_VALUE;
bool is_true(object obj);

extern const object VA_TERM;
bool is_va_term(object obj);

//
// Numbers
// =============================================================================
//

bool is_number(const object obj);
object of_integer(integer integer, meta_data meta_data);
integer to_integer(const object obj);
object of_floating(floating floating, meta_data meta_data);
floating to_floating(const object obj);

//
// Strings
// =============================================================================
//

bool is_string(const object obj);
object of_string(char *string, meta_data meta_data);
char const *to_string(const object obj);

//
// Pairs
// =============================================================================
//

bool is_pair(const object obj);
object cons(const object a, const object b, meta_data meta_data);
object car(const object obj);
object cdr(const object obj);
object set_car(object pair, const object new_car);
object set_cdr(object pair, const object new_cdr);

//
// Primitive Procedures
// =============================================================================
//

bool is_primitive_procedure(const object obj);
object of_func(object (*prim_proc)(object obj), meta_data meta_data);
object (*to_func(object obj))(object args);

//
// Symbols
// =============================================================================
//

bool is_symbol(const object obj);
// This could be called 'create_symbol' but it should normally only be called
// by obarray
object create_obarray_entry(char *id, meta_data meta_data);
object of_name(char *id, meta_data meta_data);
char const *to_name(const object obj);

//
// Symbols - Keywords
// =============================================================================
//

extern const object BEGIN;
extern const object DEFINE;
extern const object DOT;
extern const object IF;
extern const object LAMBDA;
extern const object QUASIQUOTE;
extern const object QUOTE;
extern const object UNQUOTE;

// Not keywords:

extern const object FALSE;
extern const object OK;
extern const object TRUE;

//
// Goto Labels
// =============================================================================
//

integer to_label(object obj);

#endif
