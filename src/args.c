#include "args.h"

#include "sicp-error.h"

#include <stdarg.h>

static char *plural_s(int n)
{
	return n == 1 ? "" : "s";
}

object set_args(char *caller, struct object **args_p, int count, bool at_least,
		...)
{
	va_list arg_ptrs;
	va_start(arg_ptrs, at_least);

	int i;
	for (i = 0; i < count; i++) {
		if (is_pair(*args_p)) {
			struct object **arg_p =
				va_arg(arg_ptrs, struct object **);
			*arg_p = car(*args_p);
			*args_p = cdr(*args_p);
		} else {
			va_end(arg_ptrs);
			eprintf("'%s' expected %d argument%s, but was given %d argument%s.",
				caller, count, plural_s(count), i, plural_s(i));
			object err = (of_error_kind(
				ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				NO_META_DATA));
			return err;
		}
	}
	if (!at_least && is_pair(*args_p)) {
		va_end(arg_ptrs);
		eprintf("'%s' expected exactly %d argument%s, but was given at least %d argument%s.",
			caller, count, plural_s(count), count + 1,
			plural_s(count + 1));
		return (of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
				      NO_META_DATA));
	}
	va_end(arg_ptrs);
	return ok();
}
