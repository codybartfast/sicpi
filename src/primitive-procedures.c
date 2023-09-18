#include "primitive-procedures.h"

#include "sicp-error.h"

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
		floating rslt = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			rslt += TO_NUMBER(car(args));
		}
		return of_floating(rslt, NO_META_DATA);
	} else {
		integer rslt = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			rslt += to_integer(car(args));
		}
		return of_integer(rslt, NO_META_DATA);
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
		floating rslt = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			object arg = car(args);
			rslt -= TO_NUMBER(arg);
		}
		return of_floating(rslt, NO_META_DATA);
	} else {
		integer rslt = TO_NUMBER(car(args));
		for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
			rslt -= to_integer(car(args));
		}
		return of_integer(rslt, NO_META_DATA);
	}
}

object Mul(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (have_floating) {
		floating rslt = 1;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			object arg = car(args);
			rslt *= TO_NUMBER(arg);
		}
		return of_floating(rslt, NO_META_DATA);
	} else {
		integer rslt = 1;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			rslt *= to_integer(car(args));
		}
		return of_integer(rslt, NO_META_DATA);
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

	floating rslt = TO_NUMBER(car(args));
	for (args = cdr(args); args != EMPTY_LIST; args = cdr(args)) {
		floating denominator = TO_NUMBER(car(args));
		if (denominator == 0.0f) {
			return of_error_kind(ERROR_DIVISION_BY_ZERO,
					     NO_META_DATA);
		}
		rslt /= denominator;
	}
	bool use_int =
		!have_floating && ((rslt - (integer)rslt) < floating_epsilon);
	return use_int ? of_integer(rslt, NO_META_DATA) :
			 of_floating(rslt, NO_META_DATA);
}
