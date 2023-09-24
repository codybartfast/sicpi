#include "primitive-procedures.h"

#include "args.h"
#include "sicp-error.h"

#include <stdio.h>

object Display(object args)
{
	ARGS_1("display", args);

	printf("%s", to_text(arg1));

	return OK;
}

object Newline(object args)
{
	ARGS_0("newline", args);

	printf("\n");
	return OK;
}

object Print(object args)
{
	ARGS_1("print", args);

	printf("%s\n", to_text(arg1));

	return OK;
}

object Void(object args)
{
	ARGS_0("void", args);

	return VOID_VALUE;
}

//
// ARITHMETIC
//

#define TO_NUMBER(OBJ)                                                         \
	(object_value_kind(OBJ) == VK_INTEGER ? to_integer(OBJ) :              \
						to_floating(OBJ))

static inline bool is_floating(object obj)
{
	return object_value_kind(obj) == VK_FLOATING;
}

static inline object zero(void)
{
	return of_integer(0, NO_META_DATA);
}

static inline object one(void)
{
	return of_integer(1, NO_META_DATA);
}

static object check_args(object args, int *arg_count, bool *have_floating)
{
	for (; args != EMPTY_LIST; args = cdr(args)) {
		object arg = car(args);
		if (!is_number(arg)) {
			return of_error_kind(ERROR_UNEXPECTED_TYPE,
					     NO_META_DATA);
		}
		(*arg_count)++;
		if (object_value_kind(arg) == VK_FLOATING) {
			*have_floating = true;
		}
	}
	return EMPTY_LIST;
}

object Add(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (have_floating) {
		floating prev = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			prev += TO_NUMBER(car(args));
		}
		return of_floating(prev, NO_META_DATA);
	} else {
		integer prev = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			prev += to_integer(car(args));
		}
		return of_integer(prev, NO_META_DATA);
	}
}

object Sub(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count == 0) {
		eprintf("Subtraction (-) requires at least one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (arg_count == 1) {
		args = cons(zero(), args, NO_META_DATA);
	}

	if (have_floating) {
		floating prev = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			object arg = car(args);
			prev -= TO_NUMBER(arg);
		}
		return of_floating(prev, NO_META_DATA);
	} else {
		integer prev = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			prev -= to_integer(car(args));
		}
		return of_integer(prev, NO_META_DATA);
	}
}

object Mul(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (have_floating) {
		floating prev = 1;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			object arg = car(args);
			prev *= TO_NUMBER(arg);
		}
		return of_floating(prev, NO_META_DATA);
	} else {
		integer prev = 1;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			prev *= to_integer(car(args));
		}
		return of_integer(prev, NO_META_DATA);
	}
}

object Div(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count == 0) {
		eprintf("Division (/) requires at least one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (arg_count == 1) {
		args = cons(one(), args, NO_META_DATA);
	}

	floating prev = TO_NUMBER(car(args));
	for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
		floating denominator = TO_NUMBER(car(args));
		if (denominator == (floating)0.0f) {
			return of_error_kind(ERROR_DIVISION_BY_ZERO,
					     NO_META_DATA);
		}
		prev /= denominator;
	}
	bool use_int =
		!have_floating && ((prev - (integer)prev) < floating_epsilon);
	return use_int ? of_integer(prev, NO_META_DATA) :
			 of_floating(prev, NO_META_DATA);
}

//
// Comparison
//

enum comparison_kind {
	COMPARE_GREATER_THAN = 1,
	COMPARE_LESS_THAN,
	COMPARE_EQUAL
};

static object compare(object args, enum comparison_kind kind, char *name,
		      char *symbol)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count < 2) {
		eprintf("%s (%s) requires at least two arguments.", name,
			symbol);
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	bool rslt = true;
	if (have_floating) {
		floating prev = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			floating n = TO_NUMBER(car(args));
			bool this_result;
			switch (kind) {
			case COMPARE_GREATER_THAN:
				this_result = prev > n;
				break;
			case COMPARE_LESS_THAN:
				this_result = prev < n;
				break;
			case COMPARE_EQUAL:
				this_result = prev == n;
				break;
			default:
				inyim("'compare' given unexpected kind: '%d'",
				      kind);
			}
			rslt = rslt && this_result;
			prev = n;
		}
	} else {
		integer prev = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			integer n = TO_NUMBER(car(args));
			bool this_result;
			switch (kind) {
			case COMPARE_GREATER_THAN:
				this_result = prev > n;
				break;
			case COMPARE_LESS_THAN:
				this_result = prev < n;
				break;
			case COMPARE_EQUAL:
				this_result = prev == n;
				break;
			default:
				inyim("'compare' given unexpected kind: '%d'",
				      kind);
			}
			rslt = rslt && this_result;
			prev = n;
		}
	}
	return rslt ? TRUE_VALUE : FALSE_VALUE;
}

inline object Greater_Than(object args)
{
	return compare(args, COMPARE_GREATER_THAN, "Greater Than", ">");
}

inline object Less_Than(object args)
{
	return compare(args, COMPARE_LESS_THAN, "Less Than", "<");
}

inline object Equal(object args)
{
	return compare(args, COMPARE_EQUAL, "Equal", "=");
}
