#ifndef OBJECT_H
#define OBJECT_H

#include "sicp-std.h"

#include <stdbool.h>
#include <inttypes.h>

enum error_kind { ERROR_UNSPECIFIED = 0, ERROR_LEXER, ERROR_PARSER };

typedef union object_value {
	const floating floating;
	const integer integer;
	const enum error_kind error_kind;
	const char *string;
} object_value;

// Objects should be passed by reference?: https://www.sicp-book.com/book-Z-H-22.html#%_idx_3188
typedef struct object {
	const int8_t value_kind;
	const meta_data meta_data;
	const object_value value;
} *object;

//
// General
// =============================================================================
//

meta_data object_meta_data(const object obj);
void object_free(object obj);
bool eq_(object x, object y);

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
// Identifiers
// =============================================================================
//

bool is_id(const object obj);
object from_id(char *string, meta_data meta_data);
char const *to_id(const object obj);

//
// Strings
// =============================================================================
//

bool is_string(const object obj);
object from_string(char *string, meta_data meta_data);
char const *to_string(const object obj);

#endif
