#include "obarray.h"
#include "sicp-error.h"
#include "parser.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static object parse_token(parser parser, token tkn);

void parser_init(parser parser, token_source token_source)
{
	parser->token_source = token_source;
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
	parser->error_message = "Lexer error.";
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

object reverse(object list)
{
	object rlist = Empty_List;
	for (; list != Empty_List; list = cdr(list)) {
		rlist = cons(car(list), rlist, object_meta_data(list));
	}
	return rlist;
}

static object list(parser parser)
{
	object lst = Empty_List;
	token tkn;
	object obj;
	while (token_type((tkn = token_read(parser->token_source))) > 0) {
		if (token_type(tkn) == TOKEN_LIST_CLOSE) {
			return reverse(lst);
		}
		obj = parse_token(parser, tkn);
		if (is_error(obj)) {
			return obj;
		}
		lst = cons(obj, lst, NO_META_DATA);
	}
	if (token_type(tkn) == TOKEN_EOS) {
		return parser_error(parser,
				    "List was not closed before end of file.");
	}
	return lexer_error(parser);
}

// define with dotted-tail notaion:
// 	https://www.sicp-book.com/book-Z-H-15.html#%_idx_1650
static object dot(parser parser, token tkn)
{
	UNUSED(parser);
	UNUSED(tkn);
	eprintfx(
		"Sorry haven't implemented 'define with dotted-tail notaion' yet");
	return NULL;
}

static object parse_token(parser parser, token tkn)
{
	switch (token_type(tkn)) {
	case TOKEN_IDENTIFIER:
		return from_name(token_text(tkn), NO_META_DATA);
	case TOKEN_NUMBER_INTEGER:
		return number_integer(parser, tkn);
	case TOKEN_NUMBER_DECIMAL:
		return number_decimal(parser, tkn);
	case TOKEN_LIST_OPEN:
		return list(parser);
	case TOKEN_LIST_CLOSE:
		return parser_error(parser, "List close ')' was not expected.");
	case TOKEN_STRING:
		return from_string(strdupx(token_text(tkn),
					   "parser:TOKEN_STRING"),
				   NO_META_DATA);
	case TOKEN_DOT:
		return dot(parser, tkn);
	case TOKEN_EOS:
		return Eos;
	case TOKEN_ERROR:
		return lexer_error(parser);
	case TOKEN_UNSPECIFIED:
	default:
		inyim("Parser given unexpected token type: %d",
		      token_type(tkn));
		exit(1); // keep compiler quiet
	}
}

object parse(parser parser)
{
	if (parser_is_errored(parser)) {
		return parser_error(
			parser, "Attempted to parse after an earlier error.");
	}

	token tkn = token_read(parser->token_source);
	return parse_token(parser, tkn);
}
