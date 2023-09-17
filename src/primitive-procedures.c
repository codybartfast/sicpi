#include "primitive-procedures.h"

#include "sicp-error.h"

#include <stdio.h> // todo

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
		floating acc = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			object arg = car(args);
			acc += object_value_kind(arg) == VK_INTEGER ?
				       to_integer(arg) :
				       to_floating(arg);
		}
		return of_floating(acc, NO_META_DATA);
	} else {
		integer acc = 0;
		for (; args != EMPTY_LIST; args = cdr(args)) {
			acc += to_integer(car(args));
		}
		return of_integer(acc, NO_META_DATA);
	}
}
