#ifndef OBJECT_H
#define OBJECT_H

#include "sicp-std.h"

#include <stdbool.h>
#include <inttypes.h>

enum error_kind { ERROR_UNSPECIFIED = 0, ERROR_LEXER, ERROR_PARSER };

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
	// This is typically the largest type, 2 * 64b = 128b.
	// Perhaps, if/when 'storage' is implemented it could be replaced
	// by 2 * 32b addresses into the starage array?
	struct pair pair;
} object_value;

// Objects should be passed by reference?:
// 	https://www.sicp-book.com/book-Z-H-22.html#%_idx_3188

typedef struct object {
	const int8_t value_kind;
	const meta_data meta_data;
	object_value value;
} *object;

//
// General
// =============================================================================
//

meta_data object_meta_data(const object obj);
void object_free(object obj);
bool eq(object x, object y);

//
// Errors
// =============================================================================
//

bool is_error(const object obj);
object from_error_kind(enum error_kind error_kind, meta_data meta_data);
enum error_kind to_error_kind(const object obj);

//
// Singletons
// =============================================================================
//

bool is_singleton(const object obj);
const char *to_singleton_text(const object obj);

// '() is not a symbol:
//	https://standards.scheme.org/official/r4rs.pdf
//	6.4 Symbols
extern const object Empty_List;
extern const object Eos;

//
// Numbers
// =============================================================================
//

bool is_number(const object obj);
object from_integer(integer integer, meta_data meta_data);
integer to_integer(const object obj);
object from_floating(floating floating, meta_data meta_data);
floating to_floating(const object obj);

//
// Strings
// =============================================================================
//

bool is_string(const object obj);
object from_string(char *string, meta_data meta_data);
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
// Symbols
// =============================================================================
//

bool is_symbol(const object obj);
object from_name(char *id, meta_data meta_data);
char const *to_name(const object obj);

#endif
