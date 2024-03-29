#include "object.h"

#include "obarray.h"
#include "sicp-error.h"
#include "sicp-std.h"
#include "string-builder.h"

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//
// Helpers
// =============================================================================
//

inline int8_t object_value_kind(object obj)
{
	return obj->value_kind;
}

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
	case VK_PRIMITIVE_PROCEDURE:
		return "Primitive Procedure";
	case VK_GOTO_LABEL:
		return "Goto Label";
	default:
		inyim("Don't know name of value kind: %d", value_kind);
		exit(1); // keep compiler happy
	}
}

static char *object_value_kind_name(object obj)
{
	return value_kind_name(object_value_kind(obj));
}

static inline void check_value_kind(object obj, enum value_kind expected,
				    char *caller)
{
	enum value_kind actual = object_value_kind(obj);
	if (actual != expected) {
		inyim("'%s' expected a %s, but was given a %s.", caller,
		      value_kind_name(expected), value_kind_name(actual));
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

//
// General
// =============================================================================
//

meta_data object_meta_data(object obj)
{
	return obj->meta_data;
}

#define TO_TEXT_BUFFER_LEN 1024
static char to_text_buffer[TO_TEXT_BUFFER_LEN];
// Returns a new string for the external representaion of an object.
char *to_text(object obj)
{
	switch (object_value_kind(obj)) {
		string_builder sb; // Emscripten won't allow declaration to follow label
	case VK_ERROR:
		return strdupx(error_kind_name(to_error_kind(obj)),
			       "object:to_text");
	case VK_SINGLETON:
		return strdupx(obj->value.string, "object:to_text");
	case VK_INTEGER:
		if (snprintf(to_text_buffer, TO_TEXT_BUFFER_LEN, "%lld",
			     (long long)to_integer(obj)) >=
		    TO_TEXT_BUFFER_LEN) {
			inyim("Buffer overflow converting integer to text: %lld",
			      to_integer(obj));
			exit(1);
		}
		return strdupx(to_text_buffer, "object:to_text");
	case VK_FLOATING:
		if (snprintf(to_text_buffer, TO_TEXT_BUFFER_LEN,
			     "%" LG_PRECISION "Lg",
			     (long double)to_floating(obj)) >=
		    TO_TEXT_BUFFER_LEN) {
			inyim("Buffer overflow converting floating point number to text: %lf",
			      to_floating(obj));
			exit(1);
		}
		return strdupx(to_text_buffer, "object:to_text");
	case VK_STRING:
		return strdupx(to_string(obj), "object:to_text");
	case VK_SYMBOL:
		return strdupx(to_name(obj), "object:to_text");
	case VK_PAIR:
		sb = sb_new(0);
		sb_addc(sb, '(');

		int index = 0;
		bool have_procedure;
		while (is_pair(obj)) {
			object item = car(obj);
			if (index == 0) {
				have_procedure = item == PROCEDURE;
			} else {
				sb_addc(sb, ' ');
				if (have_procedure && index == 3) {
					// Procedures refer to an environnment
					// which will reference procedures
					// which refer to environments ...
					item = of_string(
						"<ENV>",
						object_meta_data(item));
				}
			}
			sb_adds(sb, to_text(item));
			obj = cdr(obj);
			index++;
		}

		if (!is_null(obj)) {
			sb_adds(sb, " . ");
			sb_adds(sb, to_text(obj));
		}

		sb_addc(sb, ')');
		char *rslt = sb_copy(sb);
		sb_free(sb);
		return rslt;
	case VK_PRIMITIVE_PROCEDURE:
		return strdupx("<PRIMITIVE-PROCEDURE>", "object:to_text");
	default:
		inyim("'to_text' was given unexpected value_kind: %s.",
		      object_value_kind_name(obj));
		exit(1); // keep compiler happy
	}
}

//
// Equality
// =============================================================================
//

bool is_eq(const object a, const object b)
{
	return a == b;
}

//
// Errors
// =============================================================================
//

inline bool is_error(object obj)
{
	return object_value_kind(obj) == VK_ERROR;
}

inline object of_error_kind(enum error_kind error_kind, meta_data meta_data)
{
	return object_new(VK_ERROR, meta_data,
			  (object_value){ .error_kind = error_kind });
}

inline enum error_kind to_error_kind(object obj)
{
	check_value_kind(obj, VK_ERROR, "to_error_kind");
	return obj->value.error_kind;
}

char *error_kind_name(enum error_kind error_kind)
{
	switch (error_kind) {
	case ERROR_DIVISION_BY_ZERO:
		return "<DIVISION-BY-ZERO-ERROR>";
	case ERROR_EMPTY_BEGIN_SEQUENCE:
		return "<EMPTY-BEGIN-SEQUENCE-ERROR>";
	case ERROR_COND_CLAUSE_IS_NOT_VALID:
		return "<COND-CLAUSE-IS-NOT-A-LIST-ERROR>";
	case ERROR_ELSE_IS_NOT_LAST_CLAUSE:
		return "<ELSE-IS-NOT-LAST-CLAUSE-ERROR>";
	case ERROR_INCORRECT_NUMBER_OF_ARGUMENTS:
		return "<INCORRECT-NUMBER-OF-ARGUMENTS-ERROR>";
	case ERROR_LEXER:
		return "<LEXER-ERROR>";
	case ERROR_OUT_OF_BOUNDS:
		return "<OUT-OF-BOUNDS-ERROR>";
	case ERROR_PARSER:
		return "<PARSER-ERROR>";
	case ERROR_UNBOUND_VARIABLE:
		return "<UNBOUND-VARIABLE-ERROR>";
	case ERROR_UNEXPECTED_TYPE:
		return "<UNEXPECTED-TYPE-ERROR>";
	case ERROR_UNKNOWN_EXPRESSION_TYPE:
		return "<UNKNOWN-EXPRESSION-TYPE-ERROR>";
	case ERROR_UNKNOWN_PROCEDURE_TYPE:
		return "<UNKNOWN-PROCEDURE-TYPE-ERROR>";

	default:
		inyim("Don't know name of error kind: %d", error_kind);
		exit(1); // keep compiler happy
	}
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

static struct object _EMPTY_LIST = SINGLETON("()");
const object EMPTY_LIST = &_EMPTY_LIST;
inline bool is_null(const object obj)
{
	return obj == EMPTY_LIST;
}

static struct object _EOS = SINGLETON("<END-OF-SOURCE>");
const object EOS = &_EOS;
inline bool is_eos(object obj)
{
	return obj == EOS;
}

static struct object _FALSE_VALUE = SINGLETON("#f");
const object FALSE_VALUE = &_FALSE_VALUE;
inline bool is_false(object obj)
{
	return obj == FALSE_VALUE;
}

static struct object _TRUE_VALUE = SINGLETON("#t");
const object TRUE_VALUE = &_TRUE_VALUE;
inline bool is_true(object obj)
{
	return obj == TRUE_VALUE;
}
inline bool to_bool(object obj)
{
	if (obj == TRUE_VALUE) {
		return true;
	} else if (obj == FALSE_VALUE) {
		return false;
	} else {
		inyim("'to_bool' expected #f or #t but was given a %s.",
		      value_kind_name(object_value_kind(obj)));
		exit(1);
	}
}

static struct object _VA_TERM = SINGLETON("<VARIADIC-TERMINATOR>");
const object VA_TERM = &_VA_TERM;
inline bool is_va_term(object obj)
{
	return obj == VA_TERM;
}

static struct object _VOID_VALUE = SINGLETON("#void");
const object VOID_VALUE = &_VOID_VALUE;
inline bool is_void(object obj)
{
	return obj == VOID_VALUE;
}

//
// Numbers
// =============================================================================
//

inline bool is_number(object obj)
{
	enum value_kind vk = object_value_kind(obj);
	return vk == VK_INTEGER || vk == VK_FLOATING;
}

inline object of_integer(integer integer, meta_data meta_data)
{
	return object_new(VK_INTEGER, meta_data,
			  (object_value){ .integer = integer });
}

inline integer to_integer(object obj)
{
	check_value_kind(obj, VK_INTEGER, "to_integer");
	return obj->value.integer;
}

inline object of_floating(floating floating, meta_data meta_data)
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

inline object of_string(char *string, meta_data meta_data)
{
	return object_new(VK_STRING, meta_data,
			  (object_value){ .string = string });
}

// Returns the underlying char * pointer of a string object (no duplication).
inline char const *to_string(object obj)
{
	check_value_kind(obj, VK_STRING, "to_string");
	return obj->value.string;
}

//
// Pairs
// =============================================================================
//

inline bool is_pair(const object obj)
{
	return object_value_kind(obj) == VK_PAIR;
}

inline object cons(const object a, const object b, meta_data meta_data)
{
	return object_new(VK_PAIR, meta_data,
			  (object_value){ .pair = { a, b } });
}

inline object car(const object obj)
{
	check_value_kind(obj, VK_PAIR, "car");
	return obj->value.pair.car;
}

inline object cdr(const object obj)
{
	check_value_kind(obj, VK_PAIR, "cdr");
	return obj->value.pair.cdr;
}

inline object set_car(object pair, object new_car)
{
	pair->value.pair.car = new_car;
	return pair;
}

inline object set_cdr(object pair, object new_cdr)
{
	pair->value.pair.cdr = new_cdr;
	return pair;
}

//
// Primitive Procedures
// =============================================================================
//

bool is_primitive_procedure(const object obj)
{
	return object_value_kind(obj) == VK_PRIMITIVE_PROCEDURE;
}

object of_func(object (*prim_proc)(object obj), meta_data meta_data)
{
	return object_new(VK_PRIMITIVE_PROCEDURE, meta_data,
			  (object_value){ .primitive_procedure = prim_proc });
}

object (*to_func(object obj))(object args)
{
	check_value_kind(obj, VK_PRIMITIVE_PROCEDURE, "to_func");
	return obj->value.primitive_procedure;
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

inline object of_name(char *name, meta_data meta_data)
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

static struct object _BEGIN = SYMBOL("begin");
const object BEGIN = &_BEGIN;

static struct object _COND = SYMBOL("cond");
const object COND = &_COND;

static struct object _DEFINE = SYMBOL("define");
const object DEFINE = &_DEFINE;

static struct object _DOT = SYMBOL(".");
const object DOT = &_DOT;

static struct object _ELSE = SYMBOL("else");
const object ELSE = &_ELSE;

static struct object _LAMBDA = SYMBOL("lambda");
const object LAMBDA = &_LAMBDA;

static struct object _LET = SYMBOL("let");
const object LET = &_LET;

static struct object _IF = SYMBOL("if");
const object IF = &_IF;

static struct object _QUASIQUOTE = SYMBOL("quasiquote");
const object QUASIQUOTE = &_QUASIQUOTE;

static struct object _QUOTE = SYMBOL("quote");
const object QUOTE = &_QUOTE;

static struct object _TIME = SYMBOL("time");
const object TIME = &_TIME;

static struct object _UNQUOTE = SYMBOL("unquote");
const object UNQUOTE = &_UNQUOTE;

// Not keyowords:
// 	These are not keywords but are added here so we can have more concise
//	code e.g., by using 'OK' instead of 'of_name("ok", NO_META_DATA)'

static struct object _FALSE = SYMBOL("false");
const object FALSE = &_FALSE;

static struct object _OK = SYMBOL("ok");
const object OK = &_OK;

static struct object _PROCEDURE = SYMBOL("procedure");
const object PROCEDURE = &_PROCEDURE;

static struct object _TRUE = SYMBOL("true");
const object TRUE = &_TRUE;

// Above symbols need to be included in function below

void init_keywords(void)
{
	symbols = obarray_new(0);

	obarray_add_symbol(symbols, BEGIN);
	obarray_add_symbol(symbols, COND);
	obarray_add_symbol(symbols, DEFINE);
	obarray_add_symbol(symbols, DOT);
	obarray_add_symbol(symbols, ELSE);
	obarray_add_symbol(symbols, IF);
	obarray_add_symbol(symbols, LAMBDA);
	obarray_add_symbol(symbols, LET);
	obarray_add_symbol(symbols, QUASIQUOTE);
	obarray_add_symbol(symbols, QUOTE);
	obarray_add_symbol(symbols, TIME);
	obarray_add_symbol(symbols, UNQUOTE);

	// Not Keywords:

	obarray_add_symbol(symbols, FALSE);
	obarray_add_symbol(symbols, OK);
	obarray_add_symbol(symbols, PROCEDURE);
	obarray_add_symbol(symbols, TRUE);
}

//
// Goto Labels
// =============================================================================
//

inline integer to_label(object obj)
{
	check_value_kind(obj, VK_GOTO_LABEL, "to_goto_label");
	return obj->value.integer;
}
