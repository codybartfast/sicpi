#include "primitive-procedures.h"

#include "args.h"
#include "sicp-error.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

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

object Print_Lines(object args)
{
	for (; args != EMPTY_LIST; args = cdr(args)) {
		printf("%s\n", to_text(car(args)));
	}

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

object Remainder(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 2) {
		eprintf("'remainder' requires exactly two arguments.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (have_floating) {
		eprintf("'remainder' requires integer values.");
		return of_error_kind(ERROR_UNEXPECTED_TYPE, NO_META_DATA);
	}

	return of_integer(to_integer(car(args)) % to_integer(cadr(args)),
			  NO_META_DATA);
}

object Abs(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'abs' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (have_floating) {
		return of_floating(fabs(to_floating(car(args))), NO_META_DATA);
	} else {
		return of_integer(labs(to_integer(car(args))), NO_META_DATA);
	}
}

object Exp(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'exp' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	floating n = have_floating ? to_floating(car(args)) :
				     (floating)to_integer(car(args));

	return of_floating(exp((n)), NO_META_DATA);
}

object Log(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'log' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	floating n = have_floating ? to_floating(car(args)) :
				     (floating)to_integer(car(args));

	return of_floating(log((n)), NO_META_DATA);
}

//
// Comparison
//

enum comparison_kind {
	COMPARE_GREATER_THAN = 1,
	COMPARE_LESS_THAN,
	COMPARE_EQUAL,
	COMPARE_GREATER_THAN_OR_EQUAL,
	COMPARE_LESS_THAN_OR_EQUAL
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
			case COMPARE_GREATER_THAN_OR_EQUAL:
				this_result = prev >= n;
				break;
			case COMPARE_LESS_THAN_OR_EQUAL:
				this_result = prev <= n;
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
			case COMPARE_GREATER_THAN_OR_EQUAL:
				this_result = prev >= n;
				break;
			case COMPARE_LESS_THAN_OR_EQUAL:
				this_result = prev <= n;
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

inline object Greater_Than_Equal(object args)
{
	return compare(args, COMPARE_GREATER_THAN_OR_EQUAL,
		       "Greater Than or Equal", ">=");
}

inline object Less_Than_Equal(object args)
{
	return compare(args, COMPARE_LESS_THAN_OR_EQUAL, "Less Than or Equal",
		       "<=");
}

//
// Logic
//

object And(object args)
{
	int arg_count = length(args);
	if (arg_count < 2) {
		eprintf("'and' requires at least 2 arguments");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	bool rslt = true;

	for (; args != EMPTY_LIST; args = cdr(args)) {
		rslt = rslt && !is_false(car(args));
	}

	return rslt ? TRUE_VALUE : FALSE_VALUE;
}

object Or(object args)
{
	int arg_count = length(args);
	if (arg_count < 2) {
		eprintf("'or' requires at least 2 arguments");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	bool rslt = false;

	for (; args != EMPTY_LIST; args = cdr(args)) {
		rslt = rslt || !is_false(car(args));
	}

	return rslt ? TRUE_VALUE : FALSE_VALUE;
}

object Not(object args)
{
	if (!is_pair(args)) {
		eprintf("'not' requires 1 argument, but got none");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (cdr(args) != EMPTY_LIST) {
		eprintf("'not' requires 1 argument, but got at least 2");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	return car(args) == FALSE_VALUE ? TRUE_VALUE : FALSE_VALUE;
}

//
// Random
//

bool have_set_random_seed = false;
object Set_Random_Seed(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'set-random-seed' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (have_floating) {
		eprintf("'set-random-seed' requires an integer.");
		return of_error_kind(ERROR_UNEXPECTED_TYPE, NO_META_DATA);
	}

	integer given_seed = to_integer(car(args));

	unsigned int seed = given_seed;
	srand(seed);
	have_set_random_seed = true;
	return of_integer(seed, NO_META_DATA);
}

//https://stackoverflow.com/questions/55766058/how-can-i-generate-random-doubles-in-c
static double random_floating(void)
{
	uint64_t r53 = ((uint64_t)(rand()) << 21) ^ (rand() >> 2);
	double per_SO = r53 / 9007199254740991.0; // 2^53 - 1
	// This seems to produce numbers in range 0 -> 0.5 so mult by 2
	return per_SO * 2.0;
}

object Random(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'random' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (!have_set_random_seed) {
		srand((unsigned)time(NULL));
		have_set_random_seed = true;
	}

	if (have_floating) {
		floating upper = to_floating(car(args));
		double unit = random_floating();
		return of_floating(unit * upper, NO_META_DATA);
	} else {
		integer upper = to_integer(car(args));
		if (upper < 0 || upper > RAND_MAX) {
			eprintf("'random' requires a an integer between 0 and %d.",
				RAND_MAX);
			return of_error_kind(ERROR_OUT_OF_BOUNDS, NO_META_DATA);
		}
		return of_integer(rand() % upper, NO_META_DATA);
	}
}

//
// Time
//

object Runtime(object args)
{
	if (args != EMPTY_LIST) {
		eprintf("'runtime' expects no arguments.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}
	return (of_integer(clock(), NO_META_DATA));
}

object Seconds(object args)
{
	int arg_count = 0;
	bool have_floating = false;
	RETURN_IF_ERROR(check_args(args, &arg_count, &have_floating));

	if (arg_count != 1) {
		eprintf("'seconds' requires exactly one argument.");
		return of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				     NO_META_DATA);
	}

	if (have_floating) {
		eprintf("'seconds' requires an integer.");
		return of_error_kind(ERROR_UNEXPECTED_TYPE, NO_META_DATA);
	}

	return of_floating((floating)to_integer(car(args)) /
				   (floating)CLOCKS_PER_SEC,
			   NO_META_DATA);
}
