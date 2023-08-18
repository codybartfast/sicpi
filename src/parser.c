#include "parser.h"

#include <errno.h>
#include <stdlib.h>

static object number(token tkn)
{
	long long conv_int;
	char *end = NULL;
	char *s = token_text(tkn);
	errno = 0;
	conv_int = strtoll(s, &end, 10);
	// check string number doesn't overflow long long and
	// long long doesn't overflow whatever Integer is defined as.
	if (end && !*end && errno != ERANGE && integer_min <= conv_int &&
	    conv_int <= integer_max) {
		return from_integer(conv_int, NO_META_DATA);
	}
	// long double conv_flt;
	// end = NULL;
	// // errno = 0
	// conv_flt = strtold(s, &end);
	// if (end && !*end && errno != ERANGE && floating_min <= conv_flt &&
	//     conv_flt <= floating_max) {
	// 	return from_integer(-1, NO_META_DATA);
	// 	// return from_floating(conv_flt, NO_META_DATA);
	// }
	return from_integer(-1, NO_META_DATA);
}

object parse(token_source tknsrc)
{
	token tkn = token_read(tknsrc);
	switch (token_type(tkn)) {
	case TOKEN_NUMBER:
		return number(tkn);
	default:
		return from_integer(-1, NO_META_DATA);
	}
}
