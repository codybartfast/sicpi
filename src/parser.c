#include "parser.h"

#include <string.h>

#define MAX_DIGITS 18
static object number(token tkn)
{
	char *s = tkn_text(tkn);
	// size_t maxlen = (*s == '+' || *s == '-') ? MAX_DIGITS + 1 : MAX_DIGITS;
	return from_integer(atoll(s), NO_META_DATA);
}

object parse(token_source tkn_src)
{
	token tkn = tkn_src.read_token(tkn_src.state);
	switch (tkn_type(tkn)) {
	case TKN_NUMBER:
		return number(tkn);
	default:
		return from_integer(-1, NO_META_DATA);
	}
	// if (tkn_type(tkn) == TKN_NUMBER && !strcmp("486", tkn_text(tkn)))
	// 	return from_integer(234, NO_META_DATA);
	// else
	// 	return from_integer(-1, NO_META_DATA);
}
