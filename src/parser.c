#include "list.h"
#include "obarray.h"
#include "sicp-error.h"
#include "parser.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static object parse_token(parser parser, token tkn);

static char *const dot_err_msg =
	"Dot '.' can only be used for dotted-tail definitions";

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

static object lexer_error(parser parser, meta_data meta_data)
{
	parser->is_errored = true;
	parser->error_message = "Lexer error.";
	return of_error_kind(ERROR_LEXER, meta_data);
}

static object parser_error(parser parser, char *const error_message, meta_data meta_data)
{
	parser->is_errored = true;
	parser->error_message = error_message;
	return of_error_kind(ERROR_PARSER, meta_data);
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
		return of_integer(conv_int, token_key(tkn));
	}
	return parser_error(
		parser,
		"Failed to convert string to integer (digit overflow?).",
		token_key(tkn));
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
		return of_floating(conv_flt, token_key(tkn));
	}
	return parser_error(parser,
			    "Failed to convert string to decimal (overflow?).",
			    token_key(tkn));
}

static object reverse(object list)
{
	object rlist = EMPTY_LIST;
	while (list != EMPTY_LIST) {
		rlist = cons(car(list), rlist, object_meta_data(list));
		object pair = list;
		list = cdr(list);
		object_free(pair);
	}
	return rlist;
}

inline static object dot(parser parser, object lst, token tkn)
{
	if (is_null(lst)) {
		return parser_error(parser, dot_err_msg, token_key(tkn));
	}
	object tail = parse(parser);
	if (!is_symbol(tail)) {
		return parser_error(parser, dot_err_msg, token_key(tkn));
	}
	if (token_type(token_read(parser->token_source)) != TOKEN_LIST_CLOSE) {
		return parser_error(parser, dot_err_msg, token_key(tkn));
	}
	lst = cons(tail, cons(DOT, lst, token_key(tkn)), NO_META_DATA);
	return reverse(lst);
}

static object list(parser parser)
{
	object lst = EMPTY_LIST;
	token tkn;
	object obj;
	enum token_type tkn_typ;
	while ((tkn_typ = token_type(
			(tkn = token_read(parser->token_source)))) > 0) {
		if (tkn_typ == TOKEN_LIST_CLOSE) {
			return reverse(lst);
		}
		if (tkn_typ == TOKEN_DOT) {
			return dot(parser, lst, tkn);
		}
		obj = parse_token(parser, tkn);
		if (is_error(obj)) {
			return obj;
		}
		lst = cons(obj, lst, token_key(tkn));
	}
	if (token_type(tkn) == TOKEN_EOS) {
		return parser_error(parser,
				    "List was not closed before end of file.",
				    token_key(tkn));
	}
	return lexer_error(parser, token_key(tkn));
}

static object abbreviation(parser parser, char *name)
{
	object obj = parse(parser);
	if (is_error(obj) || obj == EOS) {
		return obj;
	}
	return list2(of_name(name, NO_META_DATA), obj);
}

static object parse_token(parser parser, token tkn)
{
	switch (token_type(tkn)) {
	case TOKEN_IDENTIFIER:
		return of_name(token_text(tkn), token_key(tkn));
	case TOKEN_NUMBER_INTEGER:
		return number_integer(parser, tkn);
	case TOKEN_NUMBER_DECIMAL:
		return number_decimal(parser, tkn);
	case TOKEN_STRING:
		return of_string(strdupx(token_text(tkn),
					 "parser:TOKEN_STRING"),
				 token_key(tkn));
	case TOKEN_LIST_OPEN:
		return list(parser);
	case TOKEN_LIST_CLOSE:
		return parser_error(parser, "List close ')' was not expected.", token_key(tkn));
	case TOKEN_QUASIQUOTE:
		return abbreviation(parser, "quasiquote");
	case TOKEN_QUOTE:
		return abbreviation(parser, "quote");
	case TOKEN_UNQUOTE:
		return abbreviation(parser, "unquote");
	case TOKEN_DOT:
		return parser_error(parser, dot_err_msg, token_key(tkn));
	case TOKEN_EOS:
		return EOS;
	case TOKEN_ERROR:
		return lexer_error(parser, token_key(tkn));
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
			parser, "Attempted to parse after an earlier error.",
			NO_META_DATA);
	}

	token tkn = token_read(parser->token_source);
	return parse_token(parser, tkn);
}

object parse_all(parser parser)
{
	object rslt = EMPTY_LIST;
	object obj;
	while ((obj = parse(parser)) != EOS) {
		if (is_error(obj)) {
			return obj;
		}
		rslt = cons(obj, rslt, NO_META_DATA);
	}
	return reverse(rslt);
}
