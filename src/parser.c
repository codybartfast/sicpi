#include "sicp-error.h"
#include "parser.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser_init(parser parser)
{
	parser->token_source = NULL;
	parser->is_errored = false;
	parser->error_message = NULL;
}

inline bool parser_is_errored(parser parser)
{
	return parser->is_errored;
}

inline char *parser_error_message(parser parser)
{
	return parser->error_message;
}

static object lexer_error(parser parser)
{
	parser->is_errored = true;
	return from_error_kind(ERROR_LEXER, NO_META_DATA);
}

static object parser_error(parser parser, char *error_message)
{
	parser->is_errored = true;
	parser->error_message = error_message;
	return from_error_kind(ERROR_PARSER, NO_META_DATA);
}

static object number_integer(parser parser, token tkn)
{
	long long conv_int;
	char *end = NULL;
	char *s = token_text(tkn);
	errno = 0;
	conv_int = strtoll(s, &end, 10);
	if (end && !*end && errno != ERANGE && integer_min <= conv_int &&
	    conv_int <= integer_max) {
		return from_integer(conv_int, NO_META_DATA);
	}
	return parser_error(parser,
			    "Failed to convert string to integer (overflow?).");
}

static object number_decimal(parser parser, token tkn)
{
	long double conv_flt;
	char *end = NULL;
	errno = 0;
	char *s = token_text(tkn);
	conv_flt = strtold(s, &end);
	if (end && !*end && errno != ERANGE && floating_min <= conv_flt &&
	    conv_flt <= floating_max) {
		return from_floating(conv_flt, NO_META_DATA);
	}
	return parser_error(parser,
			    "Failed to convert string to decimal (overflow?).");
}

object parse(parser parser)
{
	if (parser_is_errored(parser)) {
		return parser_error(
			parser, "Attempted to parse after an earlier error.");
	}

	token tkn = token_read(parser->token_source);
	switch (token_type(tkn)) {
	case TOKEN_NUMBER_INTEGER:
		return number_integer(parser, tkn);
	case TOKEN_NUMBER_DECIMAL:
		return number_decimal(parser, tkn);
	case TOKEN_ERROR:
		return lexer_error(parser);
	default:
		char buff[100];
		sprintf(buff, "Parser given unexpected token type: %d",
			token_type(tkn));
		char *errmsg = strdup(buff);
		if (!errmsg) {
			alloc_error("parse");
		}
		return parser_error(parser,
				    "Parser given unexpected token type");
	}
}
