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
	// This is the largest type, 2 * 64b = 128b.
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
char *to_text(object obj);

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

extern const object FALSE;
bool is_false(object obj);

extern const object TRUE;
bool is_true(object obj);

extern const object EMPTY_LIST;
bool is_empty_list(object obj);

extern const object EOS;
bool is_eos(object obj);

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

bool is_null(const object obj);
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
// This could be called 'create_symbol' but it should normally only be called
// by obarray
object create_obarray_entry(char *id, meta_data meta_data);
object of_name(char *id, meta_data meta_data);
char const *to_name(const object obj);

//
// Symbols - Keywords
// =============================================================================
//

extern const object DOT;
extern const object QUASIQUOTE;
extern const object QUOTE;
extern const object UNQUOTE;

//
// Goto Labels
// =============================================================================
//

#define GOTO_LABEL_VALUE_KIND 1

integer to_label(object obj);

#endif
